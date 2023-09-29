#include "SubSampler.h"
#include <cmath>
#include <numeric>

std::optional<QList<std::pair<qreal, qreal> > > SubSampler::addValue(qreal x, qreal y)
{
    auto timepoint = std::floor(x/samplePeriod) * samplePeriod;
    if(timepoint < currentTimepoint)
    {
        accumulator.append(y);
        return std::nullopt;
    }
    else
    {
        auto ret = advance(timepoint);
        accumulator.append(y);
        return ret;
    }
    return std::nullopt;
}

std::pair<qreal, qreal> SubSampler::finalize()
{
    std::pair<qreal, qreal> ret;
    auto mean = std::reduce(accumulator.begin(), accumulator.end()) / (samplePeriod/1000);
    updateMinMaxY(mean);
    ret = {currentTimepoint, mean};
    minmaxX.second = currentTimepoint;
    return ret;
}

QList<std::pair<qreal, qreal>> SubSampler::advance(qreal timepoint)
{
    QList<std::pair<qreal, qreal>> ret;

    while(currentTimepoint <= timepoint)
    {
        if(accumulator.empty())
        {
            updateMinMaxY(0);
            ret.append({currentTimepoint,0});
        }
        else
        {
            auto mean = std::reduce(accumulator.begin(), accumulator.end()) / (samplePeriod/1000);
            updateMinMaxY(mean);
            ret.append({currentTimepoint, mean});
        }
        accumulator.clear();
        currentTimepoint+=samplePeriod;
    }

    minmaxX.second = currentTimepoint;
    return ret;
}

void SubSampler::updateMinMaxY(qreal val)
{
    if(val < minmaxY.first)
    {
        minmaxY.first = val;
    }
    if(val > minmaxY.second)
    {
        minmaxY.second = val;
    }
}

std::pair<qreal, qreal> SubSampler::getMinmaxY() const
{
    return minmaxY;
}

std::pair<qreal, qreal> SubSampler::getMinmaxX() const
{
    return minmaxX;
}

