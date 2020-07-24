#include <algorithm>
#include <iostream>
#include <vector>

template<class T>
void print(std::vector<T> const & xs)
{
    for (std::size_t i = 0; i < xs.size(); i++)
    {
        std::cout << xs.at(i) << " ";
    }
    std::cout << std::endl;
}

struct Insertion
{
    template<class T>
    static void sort(std::vector<T> & xs)
    {
        int const len = xs.size();
        for (int i = 1; i < len; i++)
        {
            for (int j = i; j > 0; j--)
            {
                T & t1 = xs.at(j - 1);
                T & t2 = xs.at(j);

                if (t1 > t2)
                {
                    std::swap(t1, t2);
                }
                else
                {
                    break;
                }
            }
        }
    }
};

struct Selection
{
    template<class T>
    static void sort(std::vector<T> & xs) {
        int const len = xs.size();
        for (int i = 0; i < len; i++)
        {
            int min = i;

            for (int j = i + 1; j < len; j++)
            {
                T & t1 = xs.at(min);
                T & t2 = xs.at(j);

                if (t1 > t2)
                {
                    min = j;
                }
            }

            if (i != min)
            {
                std::swap(xs.at(i), xs.at(min));
            }
        }
    }
};

struct Quick
{
    template<class T>
    static void sort(std::vector<T> & xs, int min, int max) {
        if ((max - min) < 2) { return; }

        // Select a pivot from the middle of the sub-array.
        int const mid = (min + ((max - min) / 2));
        T const med = xs.at(mid);

        // Swap pivot to the end.
        std::swap(xs.at(mid), xs.at(max - 1));

        int store = min;
        for (int i = min; i < (max - 1); i++) {
            if (xs.at(i) < med) {
                std::swap(xs.at(i), xs.at(store++));
            }
        }
        std::swap(xs.at(store), xs.at(max - 1));

        sort(xs, min, store);
        sort(xs, store + 1, max);
    }

    template<class T>
    static void sort(std::vector<T> &xs)
    {
        sort(xs, 0, xs.size());
    }
};

struct Merge
{
    template<class T>
    static void sort(std::vector<T> & xs, int min, int max)
    {
        int const len = (max - min);

        if (len < 2) { return; }

        int const mid = (min + (len / 2));

        sort(xs, min, mid);
        sort(xs ,mid, max);

        std::vector<T> ms(len);

        int i1 = min, i2 = mid, i = 0;
        while ((i1 < mid) && (i2 < max)) {
            T const & x1 = xs.at(i1);
            T const & x2 = xs.at(i2);

            if (x1 <= x2) {
                ms.at(i) = x1;
                i1++;
            } else {
                ms.at(i) = x2;
                i2++;
            }

            i++;
        }

        while (i1 < mid)
        {
            ms.at(i++) = xs.at(i1++);
        }


        while (i2 < max)
        {
            ms.at(i++) = xs.at(i2++);
        }

        for (int i = 0; i < len; i++)
        {
            xs.at(min + i) = ms.at(i);
        }
    }

    template<class T>
    static void sort(std::vector<T> & xs)
    {
        sort(xs, 0, xs.size());
    }

};

template<class T>
void testSTL(std::vector<T> const & xs)
{
    std::vector<T> xs1(xs);
    std::sort(xs1.begin(), xs1.end());
    print<T>(xs1);
}

template<class T, class S>
void testOne(std::vector<T> const & xs)
{
    std::vector<T> xs1(xs);
    S::sort(xs1);
    print<T>(xs1);
}

template<class T>
void test(std::vector<T> const & xs)
{
    std::cout << "STL:" << std::endl;
    testSTL<T>(xs);
    std::cout << std::endl;

    std::cout << "Insertion:" << std::endl;
    testOne<T, Insertion>(xs);
    std::cout << std::endl;

    std::cout << "Selection:" << std::endl;
    testOne<T, Selection>(xs);
    std::cout << std::endl;

    std::cout << "Quick:" << std::endl;
    testOne<T, Quick>(xs);
    std::cout << std::endl;

    std::cout << "Merge:" << std::endl;
    testOne<T, Merge>(xs);
    std::cout << std::endl;
}

void test() {
    std::vector<int> xs;
    xs.push_back(7);
    xs.push_back(2);
    xs.push_back(3);
    xs.push_back(1);
    xs.push_back(9);
    xs.push_back(-2);
    xs.push_back(0);
    test(xs);
}

int main(int argc, char **argv) {
    test();
    return 0;
}


