/**
* @file RadixTrie_test.h.
* @brief The RadixTrie_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/RadixTrie.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief Testing Spices::radix_trie<BITS, 1>
    */
    TEST(RadixTrieTest, Layer1) {

        SPICESTEST_PROFILE_FUNCTION();

        static constexpr uint64_t n = 1 << 19;

        std::vector<uint64_t> vs;
        vs.resize(n);

        std::unordered_map<uint64_t, void*> map;
        for (uint64_t i = 0; i < n; i++)
        {
            map[i] = &vs[i];
        }

        scl::radix_trie<19, 1> rt;
        for (uint64_t i = 0; i < n; i++)
        {
            rt.set(i, &vs[i]);
        }

        {
            SPICESTEST_PROFILE_SCOPE("Hash map");

            for (uint64_t i = 0; i < n; i++)
            {
                EXPECT_NE(map.find(i), map.end());
                EXPECT_EQ(map[i], &vs[i]);
            }
        }

        {
            SPICESTEST_PROFILE_SCOPE("radix_trie<BITS, 1>");

            for (uint64_t i = 0; i < n; i++)
            {
                EXPECT_EQ(rt.get(i), &vs[i]);
            }
        }
    }

    /**
    * @brief Testing Spices::radix_trie<BITS, 2>
    */
    TEST(RadixTrieTest, Layer2) {

        SPICESTEST_PROFILE_FUNCTION();

        static constexpr uint64_t n = 1 << 19;

        std::vector<uint64_t> vs;
        vs.resize(n);

        std::unordered_map<uint64_t, void*> map;
        for (uint64_t i = 0; i < n; i++)
        {
            map[i] = &vs[i];
        }

        scl::radix_trie<19, 2> rt;
        for (uint64_t i = 0; i < n; i++)
        {
            rt.set(i, &vs[i]);
        }

        {
            SPICESTEST_PROFILE_SCOPE("Hash map");

            for (uint64_t i = 0; i < n; i++)
            {
                EXPECT_NE(map.find(i), map.end());
                EXPECT_EQ(map[i], &vs[i]);
            }
        }

        {
            SPICESTEST_PROFILE_SCOPE("radix_trie<BITS, 2>");

            for (uint64_t i = 0; i < n; i++)
            {
                void* p = rt.get(i);

                EXPECT_EQ(p, &vs[i]);
            }
        }
    }

    /**
    * @brief Testing Spices::radix_trie<BITS, 3>
    */
    TEST(RadixTrieTest, Layer3) {

        SPICESTEST_PROFILE_FUNCTION();

        static constexpr uint64_t n = 1 << 19;

        std::vector<uint64_t> vs;
        vs.resize(n);

        std::unordered_map<uint64_t, void*> map;
        for (uint64_t i = 0; i < n; i++)
        {
            map[i] = &vs[i];
        }

        scl::radix_trie<51, 3> rt;
        for (uint64_t i = 0; i < n; i++)
        {
            rt.set(i, &vs[i]);
        }

        {
            SPICESTEST_PROFILE_SCOPE("Hash map");

            for (uint64_t i = 0; i < n; i++)
            {
                EXPECT_NE(map.find(i), map.end());
                EXPECT_EQ(map[i], &vs[i]);
            }
        }

        {
            SPICESTEST_PROFILE_SCOPE("radix_trie<BITS, 3>");

            for (uint64_t i = 0; i < n; i++)
            {
                EXPECT_EQ(rt.get(i), &vs[i]);
            }
        }
    }
}