#ifndef SUBSAMPLER_H
#define SUBSAMPLER_H

#include <QList>
#include <optional>
#include <utility>

class SubSampler
{
public:
    SubSampler(qreal period, qreal startTime) : samplePeriod(period)
    {
        currentTimepoint =
            (std::floor(startTime / samplePeriod) * samplePeriod) +
            samplePeriod;
        minmaxX.first = currentTimepoint;
    }

    std::optional<QList<std::pair<qreal, qreal>>> addValue(qreal x, qreal y);

    std::pair<qreal, qreal> finalize();

    std::pair<qreal, qreal> getMinmaxX() const;

    std::pair<qreal, qreal> getMinmaxY() const;

private:
    QList<std::pair<qreal, qreal>> advance(qreal timepoint);
    void updateMinMaxY(qreal val);

    qreal samplePeriod;
    qreal currentTimepoint{0};
    QList<qreal> accumulator;
    std::pair<qreal, qreal> minmaxX{0, 0};
    std::pair<qreal, qreal> minmaxY{0, 0};
};

#endif // SUBSAMPLER_H
