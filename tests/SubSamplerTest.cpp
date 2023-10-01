#include "../filters/SubSampler.h"
#include <algorithm>
#include <boost/ut.hpp>
#include <span>
#include <utility>
#include <vector>

struct param
{
    std::vector<std::pair<qreal, qreal>> in;
    std::vector<std::pair<qreal, qreal>> out;
    qreal period;
    qreal start;
};

namespace
{
param p1{{{100, 100}}, {{1000, 100}}, 1000, 0};

param p2{{{100, 100}}, {{2000, 50}}, 2000, 0};

param p3{{{100, 100}, {200, 100}}, {{1000, 200}}, 1000, 0};

param p4{{{100, 100}, {500, 100}, {1000, 100}, {1500, 100}},
         {{1000, 200}, {2000, 200}},
         1000,
         0};

param p5{{{30100, 100}, {30500, 100}, {31000, 100}, {31500, 100}},
         {{31000, 200}, {32000, 200}},
         1000,
         30000};

param p10{{{65618, 3361},
           {65618, 2373},
           {65618, 474},
           {67125, 4673},
           {68424, 2716},
           {68424, 543},
           {68528, 1962},
           {68528, 1737},
           {69036, 3897},
           {70236, 7421},
           {70236, 2938},
           {71243, 7069},
           {71243, 1413},
           {71840, 5014},
           {73341, 2543},
           {74438, 4193},
           {74438, 838},
           {75639, 9456},
           {75639, 3744}},
          {{70000, 4347.2}, {75000, 6285.8}, {80000, 2640}},
          5000,
          65618};

param p11{{{65618, 3361},
           {65618, 2373},
           {65618, 474},
           {67125, 4673},
           {68424, 2716},
           {68424, 543},
           {68528, 1962},
           {68528, 1737},
           {69036, 3897},
           {70236, 7421},
           {70236, 2938},
           {71243, 7069},
           {71243, 1413},
           {71840, 5014},
           {73341, 2543},
           {74438, 4193},
           {74438, 838},
           {75639, 9456},
           {75639, 3744}},
          {{66000, 3104},
           {68000, 2336.5},
           {70000, 5427.5},
           {72000, 11927.5},
           {74000, 1271.5},
           {76000, 9115.5}},
          2000,
          65618};

param p12{{{65618, 3361},
           {65618, 2373},
           {65618, 474},
           {67125, 4673},
           {68424, 2716},
           {68424, 543},
           {68528, 1962},
           {68528, 1737},
           {69036, 3897},
           {70236, 7421},
           {70236, 2938},
           {71243, 7069},
           {71243, 1413},
           {71840, 5014},
           {73341, 2543},
           {74438, 4193},
           {74438, 838},
           {75639, 9456},
           {75639, 3744}},
          {
              {66000, 6208 * 2},  {66500, 0},        {67000, 0},
              {67500, 4673 * 2},  {68000, 0},        {68500, 3259 * 2},
              {69000, 3699 * 2},  {69500, 3897 * 2}, {70000, 0},
              {70500, 10359 * 2}, {71000, 0},        {71500, 8482 * 2},
              {72000, 5014 * 2},  {72500, 0},        {73000, 0},
              {73500, 2543 * 2},  {74000, 0},        {74500, 5031 * 2},
              {75000, 0},         {75500, 0},        {76000, 13200 * 2},
          },
          500,
          65618};

} // namespace

void range_compare(std::span<const std::pair<qreal, qreal>> l,
                   std::span<const std::pair<qreal, qreal>> r)
{
    boost::ut::expect(l.size() == r.size())
        << "range size mismatch, l: " << l.size()
        << ", r: " << r.size(); // << boost::ut::fatal;
    auto li = l.begin();
    auto ri = r.begin();
    for (; li != l.end(); ++li, ++ri)
    {
        boost::ut::expect(std::abs(li->first - ri->first) <= (li->first / 1000))
            << "mismatched timestamps l: " << li->first
            << ", r: " << ri->first; // << boost::ut::fatal;
        boost::ut::expect(std::abs(li->second - ri->second) <=
                          (li->second / 1000))
            << "mismatched values l: " << li->second
            << ", r: " << ri->second; // << boost::ut::fatal;
    }
}

std::vector<std::pair<qreal, qreal>>
test(std::span<const std::pair<qreal, qreal>> data, qreal period, qreal start)
{
    std::vector<std::pair<qreal, qreal>> ret;
    SubSampler sampler{period, start};

    for (const auto &[x, y] : data)
    {
        auto samples = sampler.addValue(x, y);
        if (samples.has_value())
        {
            for (const auto &sample : samples.value())
            {
                ret.emplace_back(sample);
            }
        }
    }

    ret.emplace_back(sampler.finalize());

    return ret;
}

void test_check(const param &p)
{
    auto ret = test(p.in, p.period, p.start);
    range_compare(p.out, ret);
}

int main()
{
    test_check(p1);
    test_check(p2);
    test_check(p3);
    test_check(p4);
    test_check(p5);
    test_check(p10);
    test_check(p11);
    test_check(p12);

    return 0;
}
