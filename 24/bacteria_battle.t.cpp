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

    SECTION("Effective Power")
    {
        char const sample_group[] = "Immune System:" "\n"
            "18 units each with 729 hit points (weak to fire; immune to cold, slashing)"
            " with an attack that does 8 radiation damage at initiative 10";
        auto b = parseInput(sample_group);

        CHECK(b.effectivePower(0) == 144);
    }

    SECTION("Attack Damage")
    {
        INFO("By default, an attacking group would deal damage equal to its effective power to the defending group.");
        Battlefield b;
        b.groups.push_back(Group{ UnitStats{ Faction::Immune, 4000, 32, AttackType::Cold, 1, {}, {} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 32, AttackType::Fire, 2, {}, {} }, 123 });
        CHECK(b.attackDamage(0, 1) == b.effectivePower(0));

        INFO("However, if the defending group is immune to the attacking group's attack type, the defending group instead takes no damage;");
        b.groups[1].stats.immunities.push_back(b.groups[0].stats.attackType);
        CHECK(b.attackDamage(0, 1) == 0);

        INFO("If the defending group is weak to the attacking group's attack type, the defending group instead takes double damage.");
        b.groups[1].stats.immunities.clear();
        b.groups[1].stats.weaknesses.push_back(b.groups[0].stats.attackType);
        CHECK(b.attackDamage(0, 1) == 2 * b.effectivePower(0));
    }

    SECTION("Target Selection Phase")
    {
        Battlefield b;
        b.groups.push_back(Group{ UnitStats{ Faction::Immune, 4000, 32, AttackType::Cold, 1, {}, {} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 32, AttackType::Fire, 2, {}, {} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 32, AttackType::Fire, 3, {AttackType::Cold}, {} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 33, AttackType::Fire, 4, {}, {} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 32, AttackType::Fire, 5, {}, {} }, 123 });

        INFO("The attacking group chooses to target the group in the enemy army to which it would deal the most damage");
        b.targetSelection();
        CHECK(b.selected_targets[0] == 2);

        INFO("If an attacking group is considering two defending groups to which it would deal equal damage," \
            " it chooses to target the defending group with the largest effective power");
        b.groups[2].stats.weaknesses.clear();
        b.targetSelection();
        CHECK(b.selected_targets[0] == 3);

        INFO("if there is still a tie, it chooses the defending group with the highest initiative");
        b.groups[3].stats.attackDamage = b.groups[1].stats.attackDamage;
        b.targetSelection();
        CHECK(b.selected_targets[0] == 4);
    }

    SECTION("Battle Simulation")
    {
        Battlefield b = parseInput(sample_input);

        /* round 1
        Immune System:
        Group 1 contains 17 units
        Group 2 contains 989 units
        Infection:
        Group 1 contains 801 units
        Group 2 contains 4485 units

        Infection group 1 would deal defending group 1 185832 damage
        Infection group 1 would deal defending group 2 185832 damage
        Infection group 2 would deal defending group 2 107640 damage
        Immune System group 1 would deal defending group 1 76619 damage
        Immune System group 1 would deal defending group 2 153238 damage
        Immune System group 2 would deal defending group 1 24725 damage

        Infection group 2 attacks defending group 2, killing 84 units
        Immune System group 2 attacks defending group 1, killing 4 units
        Immune System group 1 attacks defending group 2, killing 51 units
        Infection group 1 attacks defending group 1, killing 17 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 3);
        CHECK(b.selected_targets[1] == 2);
        CHECK(b.selected_targets[2] == 0);
        CHECK(b.selected_targets[3] == 1);

        CHECK(b.groups[0].units == 17);
        CHECK(b.groups[1].units == 989);
        CHECK(b.groups[2].units == 801);
        CHECK(b.groups[3].units == 4485);
        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 905);
        CHECK(b.groups[1].units == 797);
        CHECK(b.groups[2].units == 4434);

        /* round 2
        Immune System:
        Group 2 contains 905 units
        Infection:
        Group 1 contains 797 units
        Group 2 contains 4434 units
        
        Infection group 1 would deal defending group 2 184904 damage
        Immune System group 2 would deal defending group 1 22625 damage
        Immune System group 2 would deal defending group 2 22625 damage
        
        Immune System group 2 attacks defending group 1, killing 4 units
        Infection group 1 attacks defending group 2, killing 144 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 761);
        CHECK(b.groups[1].units == 793);
        CHECK(b.groups[2].units == 4434);

        /* round 3
        Immune System:
        Group 2 contains 761 units
        Infection:
        Group 1 contains 793 units
        Group 2 contains 4434 units
        
        Infection group 1 would deal defending group 2 183976 damage
        Immune System group 2 would deal defending group 1 19025 damage
        Immune System group 2 would deal defending group 2 19025 damage
        
        Immune System group 2 attacks defending group 1, killing 4 units
        Infection group 1 attacks defending group 2, killing 143 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 618);
        CHECK(b.groups[1].units == 789);
        CHECK(b.groups[2].units == 4434);

        /* roung 4
        Immune System:
        Group 2 contains 618 units
        Infection:
        Group 1 contains 789 units
        Group 2 contains 4434 units
        
        Infection group 1 would deal defending group 2 183048 damage
        Immune System group 2 would deal defending group 1 15450 damage
        Immune System group 2 would deal defending group 2 15450 damage
        
        Immune System group 2 attacks defending group 1, killing 3 units
        Infection group 1 attacks defending group 2, killing 143 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 475);
        CHECK(b.groups[1].units == 786);
        CHECK(b.groups[2].units == 4434);

        /* round 5
        Immune System:
        Group 2 contains 475 units
        Infection:
        Group 1 contains 786 units
        Group 2 contains 4434 units
        
        Infection group 1 would deal defending group 2 182352 damage
        Immune System group 2 would deal defending group 1 11875 damage
        Immune System group 2 would deal defending group 2 11875 damage
        
        Immune System group 2 attacks defending group 1, killing 2 units
        Infection group 1 attacks defending group 2, killing 142 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 333);
        CHECK(b.groups[1].units == 784);
        CHECK(b.groups[2].units == 4434);

        /* round 6
        Immune System:
        Group 2 contains 333 units
        Infection:
        Group 1 contains 784 units
        Group 2 contains 4434 units

        Infection group 1 would deal defending group 2 181888 damage
        Immune System group 2 would deal defending group 1 8325 damage
        Immune System group 2 would deal defending group 2 8325 damage

        Immune System group 2 attacks defending group 1, killing 1 unit
        Infection group 1 attacks defending group 2, killing 142 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 191);
        CHECK(b.groups[1].units == 783);
        CHECK(b.groups[2].units == 4434);

        /* round 7
        Immune System:
        Group 2 contains 191 units
        Infection:
        Group 1 contains 783 units
        Group 2 contains 4434 units

        Infection group 1 would deal defending group 2 181656 damage
        Immune System group 2 would deal defending group 1 4775 damage
        Immune System group 2 would deal defending group 2 4775 damage

        Immune System group 2 attacks defending group 1, killing 1 unit
        Infection group 1 attacks defending group 2, killing 142 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 49);
        CHECK(b.groups[1].units == 782);
        CHECK(b.groups[2].units == 4434);

        /* round 8
        Immune System:
        Group 2 contains 49 units
        Infection:
        Group 1 contains 782 units
        Group 2 contains 4434 units

        Infection group 1 would deal defending group 2 181424 damage
        Immune System group 2 would deal defending group 1 1225 damage
        Immune System group 2 would deal defending group 2 1225 damage

        Immune System group 2 attacks defending group 1, killing 0 units
        Infection group 1 attacks defending group 2, killing 49 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 1);
        CHECK(b.selected_targets[1] == 0);
        CHECK(b.selected_targets[2] == -1);

        CHECK(b.attackPhase());
        CHECK(b.groups.size() == 2);
        CHECK(b.groups[0].units == 782);
        CHECK(b.groups[1].units == 4434);
    }

    SECTION("Simulate Battle")
    {
        Battlefield b = parseInput(sample_input);
        CHECK(b.simulateBattle() == 5216);
    }

    SECTION("Tied Battle")
    {
        Battlefield b;
        b.groups.push_back(Group{ UnitStats{ Faction::Immune, 4000, 32, AttackType::Cold, 1, {}, {AttackType::Fire} }, 123 });
        b.groups.push_back(Group{ UnitStats{ Faction::Infection, 4000, 32, AttackType::Fire, 2, {}, {AttackType::Cold} }, 123 });
        CHECK(b.simulateBattle() == -1);
    }

    SECTION("Battle with Boost")
    {
        Battlefield b = parseInput(sample_input);
        b.boostUnits(Faction::Immune, 1570);
        CHECK(b.groups[0].stats.attackDamage == 6077);
        CHECK(b.groups[1].stats.attackDamage == 1595);
        CHECK(b.groups[2].stats.attackDamage == 116);
        CHECK(b.groups[3].stats.attackDamage == 12);

        /* round 1
        Immune System:
        Group 2 contains 989 units
        Group 1 contains 17 units
        Infection:
        Group 1 contains 801 units
        Group 2 contains 4485 units

        Infection group 1 would deal defending group 2 185832 damage
        Infection group 1 would deal defending group 1 185832 damage
        Infection group 2 would deal defending group 1 53820 damage
        Immune System group 2 would deal defending group 1 1577455 damage
        Immune System group 2 would deal defending group 2 1577455 damage
        Immune System group 1 would deal defending group 2 206618 damage

        Infection group 2 attacks defending group 1, killing 9 units
        Immune System group 2 attacks defending group 1, killing 335 units
        Immune System group 1 attacks defending group 2, killing 32 units
        Infection group 1 attacks defending group 2, killing 84 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 3);
        CHECK(b.selected_targets[1] == 2);
        CHECK(b.selected_targets[2] == 1);
        CHECK(b.selected_targets[3] == 0);

        CHECK(b.groups[0].units == 17);
        CHECK(b.groups[1].units == 989);
        CHECK(b.groups[2].units == 801);
        CHECK(b.groups[3].units == 4485);
        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 4);
        CHECK(b.groups[0].units == 8);
        CHECK(b.groups[1].units == 905);
        CHECK(b.groups[2].units == 466);
        CHECK(b.groups[3].units == 4453);

        /* round 2
        Immune System:
        Group 2 contains 905 units
        Group 1 contains 8 units
        Infection:
        Group 1 contains 466 units
        Group 2 contains 4453 units

        Infection group 1 would deal defending group 2 108112 damage
        Infection group 1 would deal defending group 1 108112 damage
        Infection group 2 would deal defending group 1 53436 damage
        Immune System group 2 would deal defending group 1 1443475 damage
        Immune System group 2 would deal defending group 2 1443475 damage
        Immune System group 1 would deal defending group 2 97232 damage

        Infection group 2 attacks defending group 1, killing 8 units
        Immune System group 2 attacks defending group 1, killing 306 units
        Infection group 1 attacks defending group 2, killing 29 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 3);
        CHECK(b.selected_targets[1] == 2);
        CHECK(b.selected_targets[2] == 1);
        CHECK(b.selected_targets[3] == 0);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 876);
        CHECK(b.groups[1].units == 160);
        CHECK(b.groups[2].units == 4453);

        /* round 3
        Immune System:
        Group 2 contains 876 units
        Infection:
        Group 2 contains 4453 units
        Group 1 contains 160 units

        Infection group 2 would deal defending group 2 106872 damage
        Immune System group 2 would deal defending group 2 1397220 damage
        Immune System group 2 would deal defending group 1 1397220 damage

        Infection group 2 attacks defending group 2, killing 83 units
        Immune System group 2 attacks defending group 2, killing 427 units
        */
        b.targetSelection();
        CHECK(b.selected_targets[0] == 2);
        CHECK(b.selected_targets[1] == -1);
        CHECK(b.selected_targets[2] == 0);

        CHECK(!b.attackPhase());
        CHECK(b.groups.size() == 3);
        CHECK(b.groups[0].units == 793);
        CHECK(b.groups[1].units == 160);
        CHECK(b.groups[2].units == 4026);

        // after a few more fights...
        CHECK(b.simulateBattle() == 51);
        CHECK(b.groups[0].stats.faction == Faction::Immune);
    }

    SECTION("Find Smallest Boost")
    {
        Battlefield b = parseInput(sample_input);

        CHECK(findSmallestBoost(b) == 51);
    }
}
