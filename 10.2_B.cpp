#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>   
using namespace std;

enum Specialnist {
    KOMP_NAUKY, INFORMATYKA, MATEMATYKA_EKONOMIKA,
    FIZYKA_INFORMATYKA, TRUDOVE_NAVCHANNIA
};

struct Student
{
    string      prizv;        
    int         kurs;         
    Specialnist spec;         
    int         fizyka;       
    int         matematyka;   
    union                     
    {
        int programuvannya;   
        int chyselniMetody;   
        int pedahohika;       
    };
};

string SpecialnistToStr(const Specialnist spec);
int    ReadInt(const string prompt, const int min, const int max);
int    ThirdGrade(const Student& s);
void   Create(Student* p, const int N);
void   Print(Student* p, const int N);
void   Sort(Student* p, const int N);
int    BinSearch(Student* p, const int N, const string prizv,
    const Specialnist spec, const int grade);
int* IndexSort(Student* p, const int N);
void   PrintIndexSorted(Student* p, int* I, const int N);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int N = ReadInt("Введіть кількість студентів N: ", 1, 1000);
    Student* p = new Student[N];      

    string prizv;
    Specialnist spec;
    int grade, found;
    int* I = nullptr;

    int menuItem;
    do {
        cout << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - бінарний пошук студента за прізвищем, спеціальністю" << endl;
        cout << "       та оцінкою з третього предмету" << endl;
        cout << " [5] - індексне впорядкування та вивід даних" << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        menuItem = ReadInt("Введіть значення: ", 0, 5);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            break;
        case 2:
            Print(p, N);
            break;
        case 3:
            Sort(p, N);
            cout << "Масив фізично впорядковано." << endl;
            break;
        case 4:
            cout << "Введіть ключі пошуку:" << endl;
            cout << "    прізвище: ";
            cin.ignore(10000, '\n');
            getline(cin, prizv);
            spec = (Specialnist)ReadInt("    спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, "
                "2 - Математика та економіка, 3 - Фізика та інформатика, "
                "4 - Трудове навчання): ", 0, 4);
            grade = ReadInt("    оцінка з третього предмету: ", 0, 5);
            cout << endl;
            if ((found = BinSearch(p, N, prizv, spec, grade)) != -1)
                cout << "Знайдено студента в позиції " << found + 1 << endl;
            else
                cout << "Шуканого студента не знайдено" << endl;
            break;
        case 5:
            I = IndexSort(p, N);
            PrintIndexSorted(p, I, N);
            delete[] I;
            I = nullptr;
            break;
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}

string SpecialnistToStr(const Specialnist spec)
{
    switch (spec)
    {
    case KOMP_NAUKY:           return "Комп'ютерні науки";
    case INFORMATYKA:          return "Інформатика";
    case MATEMATYKA_EKONOMIKA: return "Математика та економіка";
    case FIZYKA_INFORMATYKA:   return "Фізика та інформатика";
    default:                   return "Трудове навчання";
    }
}

int ReadInt(const string prompt, const int min, const int max)
{
    int x;
    while (true)
    {
        cout << prompt;
        if (cin >> x && x >= min && x <= max)
            return x;
        cout << "Помилка! Введіть ціле число від " << min << " до " << max << "." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

int ThirdGrade(const Student& s)
{
    switch (s.spec)
    {
    case KOMP_NAUKY:  return s.programuvannya;
    case INFORMATYKA: return s.chyselniMetody;
    default:          return s.pedahohika;
    }
}

void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;

        cin.ignore(10000, '\n');   
        cout << "    прізвище: ";  getline(cin, p[i].prizv);
        p[i].kurs = ReadInt("    курс (1-6): ", 1, 6);
        p[i].spec = (Specialnist)ReadInt("    спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, "
            "2 - Математика та економіка, 3 - Фізика та інформатика, "
            "4 - Трудове навчання): ", 0, 4);
        p[i].fizyka = ReadInt("    оцінка з фізики: ", 0, 5);
        p[i].matematyka = ReadInt("    оцінка з математики: ", 0, 5);

        switch (p[i].spec)
        {
        case KOMP_NAUKY:
            p[i].programuvannya = ReadInt("    оцінка з програмування: ", 0, 5);
            break;
        case INFORMATYKA:
            p[i].chyselniMetody = ReadInt("    оцінка з чисельних методів: ", 0, 5);
            break;
        default:
            p[i].pedahohika = ReadInt("    оцінка з педагогіки: ", 0, 5);
            break;
        }
        cout << endl;
    }
}

static void PrintRow(const Student& s, const int num)
{
    cout << "| " << setw(2) << right << num << " "
        << "| " << setw(15) << left << s.prizv << " "
        << "| " << setw(4) << right << s.kurs << " "
        << "| " << setw(23) << left << SpecialnistToStr(s.spec) << " "
        << "| " << setw(6) << right << s.fizyka << " "
        << "| " << setw(6) << right << s.matematyka << " ";
    switch (s.spec)
    {
    case KOMP_NAUKY:
        cout << "| " << setw(8) << right << s.programuvannya << " "
            << "| " << setw(8) << " " << " "
            << "| " << setw(8) << " " << " |" << endl;
        break;
    case INFORMATYKA:
        cout << "| " << setw(8) << " " << " "
            << "| " << setw(8) << right << s.chyselniMetody << " "
            << "| " << setw(8) << " " << " |" << endl;
        break;
    default:
        cout << "| " << setw(8) << " " << " "
            << "| " << setw(8) << " " << " "
            << "| " << setw(8) << right << s.pedahohika << " |" << endl;
        break;
    }
}

static void PrintHeader(const string title)
{
    cout << title << endl;
    cout << string(108, '=') << endl;
    cout << "| " << setw(2) << right << "№" << " "
        << "| " << setw(15) << left << "Прізвище" << " "
        << "| " << setw(4) << right << "Курс" << " "
        << "| " << setw(23) << left << "Спеціальність" << " "
        << "| " << setw(6) << right << "Фізика" << " "
        << "| " << setw(6) << right << "Матем." << " "
        << "| " << setw(8) << right << "Програм." << " "
        << "| " << setw(8) << right << "Чис.мет." << " "
        << "| " << setw(8) << right << "Педагог." << " |" << endl;
    cout << string(108, '-') << endl;
}

void Print(Student* p, const int N)
{
    PrintHeader("Список студентів");
    for (int i = 0; i < N; i++)
        PrintRow(p[i], i + 1);
    cout << string(108, '=') << endl << endl;
}

void Sort(Student* p, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if ((ThirdGrade(p[i1]) > ThirdGrade(p[i1 + 1]))
                ||
                (ThirdGrade(p[i1]) == ThirdGrade(p[i1 + 1]) &&
                    p[i1].spec > p[i1 + 1].spec)
                ||
                (ThirdGrade(p[i1]) == ThirdGrade(p[i1 + 1]) &&
                    p[i1].spec == p[i1 + 1].spec &&
                    p[i1].prizv > p[i1 + 1].prizv))
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
}

int BinSearch(Student* p, const int N, const string prizv,
    const Specialnist spec, const int grade)
{
    int L = 0, R = N - 1, m;
    while (L <= R)
    {
        m = (L + R) / 2;
        if (ThirdGrade(p[m]) == grade && p[m].spec == spec && p[m].prizv == prizv)
            return m;
        if ((ThirdGrade(p[m]) < grade)
            ||
            (ThirdGrade(p[m]) == grade && p[m].spec < spec)
            ||
            (ThirdGrade(p[m]) == grade && p[m].spec == spec && p[m].prizv < prizv))
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}

int* IndexSort(Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;

    int i, j, value;
    for (i = 1; i < N; i++)
    {
        value = I[i];
        for (j = i - 1;
            j >= 0 && ((ThirdGrade(p[I[j]]) > ThirdGrade(p[value])) ||
                (ThirdGrade(p[I[j]]) == ThirdGrade(p[value]) &&
                    p[I[j]].spec > p[value].spec) ||
                (ThirdGrade(p[I[j]]) == ThirdGrade(p[value]) &&
                    p[I[j]].spec == p[value].spec &&
                    p[I[j]].prizv > p[value].prizv));
            j--)
        {
            I[j + 1] = I[j];
        }
        I[j + 1] = value;
    }
    return I;
}

void PrintIndexSorted(Student* p, int* I, const int N)
{
    PrintHeader("Список студентів (індексне впорядкування)");
    for (int i = 0; i < N; i++)
        PrintRow(p[I[i]], i + 1);
    cout << string(108, '=') << endl << endl;
}