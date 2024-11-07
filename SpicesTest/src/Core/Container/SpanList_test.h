/**
* @file SpanList_test.h.
* @brief The SpanList_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/SpanList.h>
#include "Instrumentor.h"

namespace SpicesTest {

    /**
    * @brief The interface is inherited from testing::Test.
    * Registry on Initialize.
    */
    class span_list_test : public testing::Test
    {
    protected:

        /**
        * @brief The interface is inherited from testing::Test.
        * Registry on Initialize.
        */
        void SetUp() override {}

        /**
        * @brief The interface is inherited from testing::Test.
        * Call before Destructor.
        */
        void TearDown() override {}

        scl::span_list m_SpanList;
    };

    /**
    * @brief Testing scl::span_list::Initialize.
    */
    TEST_F(span_list_test, Initialize) {

        SPICESTEST_PROFILE_FUNCTION();

        EXPECT_EQ(m_SpanList.Empty(), true);
    }

    /**
    * @brief Testing scl::span_list::Insert/Erase.
    */
    TEST_F(span_list_test, InsertErase) {

        SPICESTEST_PROFILE_FUNCTION();

        scl::span s0;
        scl::span s1;
        scl::span s2;

        {
            m_SpanList.Insert(m_SpanList.Begin(), &s0);
            EXPECT_EQ(s0.m_Prev, m_SpanList.End());
            EXPECT_EQ(s0.m_Next, m_SpanList.End());

            m_SpanList.Insert(&s0, &s1);
            EXPECT_EQ(s1.m_Prev, m_SpanList.End());
            EXPECT_EQ(s1.m_Next, &s0);
            EXPECT_EQ(s0.m_Prev, &s1);

            m_SpanList.Insert(&s0, &s2);
            EXPECT_EQ(s2.m_Prev, &s1);
            EXPECT_EQ(s2.m_Next, &s0);
            EXPECT_EQ(s1.m_Next, &s2);
            EXPECT_EQ(s0.m_Prev, &s2);
        }

        {
            m_SpanList.Erase(&s2);
            EXPECT_EQ(s2.m_Prev, nullptr);
            EXPECT_EQ(s2.m_Next, nullptr);
            EXPECT_EQ(s1.m_Next, &s0);
            EXPECT_EQ(s0.m_Prev, &s1);

            m_SpanList.Erase(&s0);
            EXPECT_EQ(s0.m_Prev, nullptr);
            EXPECT_EQ(s0.m_Next, nullptr);
            EXPECT_EQ(s1.m_Next, m_SpanList.End());

            m_SpanList.Erase(&s1);
            EXPECT_EQ(s1.m_Prev, nullptr);
            EXPECT_EQ(s1.m_Next, nullptr);
            EXPECT_EQ(m_SpanList.Empty(), true);
        }
    }

    /**
    * @brief Testing scl::span_list::PushFront/PopFront.
    */
    TEST_F(span_list_test, PushFrontPopFront) {

        SPICESTEST_PROFILE_FUNCTION();

        scl::span s0;
        scl::span s1;
        scl::span s2;

        {
            m_SpanList.PushFront(&s0);
            EXPECT_EQ(s0.m_Prev, m_SpanList.End());
            EXPECT_EQ(s0.m_Next, m_SpanList.End());

            m_SpanList.PushFront(&s1);
            EXPECT_EQ(s1.m_Prev, m_SpanList.End());
            EXPECT_EQ(s1.m_Next, &s0);
            EXPECT_EQ(s0.m_Prev, &s1);

            m_SpanList.PushFront(&s2);
            EXPECT_EQ(s2.m_Prev, m_SpanList.End());
            EXPECT_EQ(s2.m_Next, &s1);
            EXPECT_EQ(s1.m_Prev, &s2);
        }

        {
            scl::span* ps2 = m_SpanList.PopFront();
            EXPECT_EQ(ps2->m_Prev, nullptr);
            EXPECT_EQ(ps2->m_Next, nullptr);
            EXPECT_EQ(ps2, &s2);
            EXPECT_EQ(m_SpanList.Begin(), &s1);

            scl::span* ps1 = m_SpanList.PopFront();
            EXPECT_EQ(ps1->m_Prev, nullptr);
            EXPECT_EQ(ps1->m_Next, nullptr);
            EXPECT_EQ(ps1, &s1);
            EXPECT_EQ(m_SpanList.Begin(), &s0);

            scl::span* ps0 = m_SpanList.PopFront();
            EXPECT_EQ(ps0->m_Prev, nullptr);
            EXPECT_EQ(ps0->m_Next, nullptr);
            EXPECT_EQ(ps0, &s0);
            EXPECT_EQ(m_SpanList.Empty(), true);
        }
    }
}