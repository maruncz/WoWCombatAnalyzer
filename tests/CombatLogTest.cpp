#include "../CombatLog/CombatLog.h"
#include <QString>
#include <gtest/gtest.h>

struct param
{
    QString filename;
    CombatLog result;
};

namespace
{
param comma{
    ":/test-data/comma.txt",
    CombatLog{
        QList<LogLine>{LogLine{
            QDateTime::fromSecsSinceEpoch(0),
            {0, "nil", {0x80000000}},
            {0x00000000000291EC, "test", {0x514}},
            SubEvent::ENCHANT_APPLIED,
            variant_t{EnchantApplied{
                "Grand Firestone",
                Item{50427, "Bloodsurge, Kel'Thuzad's Blade of Agony"}}}}},
        QStringList{}, QStringList{"test"}}};

param simple{
    ":/test-data/simple.txt",
    CombatLog{
        QList<LogLine>{LogLine{
            QDateTime::fromString("2023/9/23 09:11:30.795",
                                  "yyyy/M/dd hh:mm:ss.zzz"),
            {0x000000000002FCA4, "test", {0x511}},
            {0xF1300079A80005AB, "Grandmaster's Training Dummy", {0x10a28}},
            SubEvent::SPELL_CAST_SUCCESS,
            variant_t{SpellCastSucces{
                detail::prefix::Spell{SpellInfo{"Moonfire", 48463, 0x40}},
                {}}}}},
        QStringList{"test"}, QStringList{"Grandmaster's Training Dummy"}}};
} // namespace

class CombatLogTest : public testing::TestWithParam<param>
{
};

TEST_P(CombatLogTest, parse)
{
    const auto &param = GetParam();
    auto res          = CombatLog::fromFile(param.filename);
    ASSERT_EQ(res, param.result);
}

INSTANTIATE_TEST_SUITE_P(CombatLog, CombatLogTest,
                         testing::Values(simple, comma));
