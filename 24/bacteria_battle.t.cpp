#include <bacteria_battle.hpp>

#include <catch.hpp>

TEST_CASE("Bacteria Battle")
{
    char const sample_input[] = "Immune System:" "\n"
                                "17 units each with 5390 hit points (weak to radiation, bludgeoning) with"
                                    " an attack that does 4507 fire damage at initiative 2" "\n"
                                "989 units each with 1274 hit points (immune to fire; weak to bludgeoning,"
                                    " slashing) with an attack that does 25 slashing damage at initiative 3" "\n"
                                "\n"
                                "Infection:" "\n"
                                "801 units each with 4706 hit points (weak to radiation) with an attack"
                                    " that does 116 bludgeoning damage at initiative 1" "\n"
                                "4485 units each with 2961 hit points (immune to radiation; weak to fire,"
                                    " cold) with an attack that does 12 slashing damage at initiative 4" "\n";

    SECTION("Parse Input")
    {
        Battlefield const b = parseInput(sample_input);
        REQUIRE(b.groups.size() == 4);
        CHECK(b.groups[0].units == 17);
        CHECK(b.groups[0].stats.faction == Faction::Immune);
        CHECK(b.groups[0].stats.hitPoints == 5390);
        CHECK(b.groups[0].stats.immunities.empty());
        CHECK(b.groups[0].stats.weaknesses == std::vector<AttackType>{ AttackType::Radiation, AttackType::Bludgeoning });
        CHECK(b.groups[0].stats.attackDamage == 4507);
        CHECK(b.groups[0].stats.attackType == AttackType::Fire);
        CHECK(b.groups[0].stats.initiative == 2);

        CHECK(b.groups[1].units == 989);
        CHECK(b.groups[1].stats.faction == Faction::Immune);
        CHECK(b.groups[1].stats.hitPoints == 1274);
        CHECK(b.groups[1].stats.immunities == std::vector<AttackType>{ AttackType::Fire });
        CHECK(b.groups[1].stats.weaknesses == std::vector<AttackType>{ AttackType::Bludgeoning, AttackType::Slashing });
        CHECK(b.groups[1].stats.attackDamage == 25);
        CHECK(b.groups[1].stats.attackType == AttackType::Slashing);
        CHECK(b.groups[1].stats.initiative == 3);

        CHECK(b.groups[2].units == 801);
        CHECK(b.groups[2].stats.faction == Faction::Infection);
        CHECK(b.groups[2].stats.hitPoints == 4706);
        CHECK(b.groups[2].stats.immunities.empty());
        CHECK(b.groups[2].stats.weaknesses == std::vector<AttackType>{ AttackType::Radiation });
        CHECK(b.groups[2].stats.attackDamage == 116);
        CHECK(b.groups[2].stats.attackType == AttackType::Bludgeoning);
        CHECK(b.groups[2].stats.initiative == 1);

        CHECK(b.groups[3].units == 4485);
        CHECK(b.groups[3].stats.faction == Faction::Infection);
        CHECK(b.groups[3].stats.hitPoints == 2961);
        CHECK(b.groups[3].stats.immunities == std::vector<AttackType>{ AttackType::Radiation });
        CHECK(b.groups[3].stats.weaknesses == std::vector<AttackType>{ AttackType::Fire, AttackType::Cold });
        CHECK(b.groups[3].stats.attackDamage == 12);
        CHECK(b.groups[3].stats.attackType == AttackType::Slashing);
        CHECK(b.groups[3].stats.initiative == 4);
    }

    SECTION("Deserialize Attack Type")
    {
        CHECK(deserialize("bludgeoning") == AttackType::Bludgeoning);
        CHECK(deserialize("cold")        == AttackType::Cold);
        CHECK(deserialize("fire")        == AttackType::Fire);
        CHECK(deserialize("radiation")   == AttackType::Radiation);
        CHECK(deserialize("slashing")    == AttackType::Slashing);
    }
}
