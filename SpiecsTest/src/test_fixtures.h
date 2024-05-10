//#pragma once
//#include "gmock/gmock.h"
//
//template <typename E>  // E is the element type.
//class Queue {
//public:
//	Queue() {
//		m_Size = 0;
//		m_DataBegin = nullptr;
//		m_DataHere = nullptr;
//	}
//
//	void Enqueue(const E& element) {
//		if (m_Size == 0)
//		{
//			m_DataBegin = (E*)malloc(sizeof(E));
//			*m_DataBegin = element;
//
//			m_DataHere = m_DataBegin;
//			
//			m_DataHere++;
//
//			m_Size++;
//		}
//		else
//		{
//			*m_DataHere = element;
//			m_Size++;
//			m_DataHere++;
//		}
//	}
//
//	E* Dequeue() {
//		if (m_Size > 0)
//		{
//			E* data = m_DataBegin;
//			m_DataBegin++;
//			m_Size--;
//			return data;
//		}
//		else
//		{
//			return nullptr;
//		}
//	}
//
//	size_t size() const {
//		return m_Size;
//	}
//private:
//	E* m_DataBegin;
//	E* m_DataHere;
//	size_t m_Size;
//};
//
//class QueueTest : public testing::Test {
//protected:
//	void SetUp() override {
//		// q0_ remains empty
//		q1_.Enqueue(1);
//		q2_.Enqueue(2);
//		q2_.Enqueue(3);
//	}
//
//	// void TearDown() override {}
//
//	Queue<int> q0_;
//	Queue<int> q1_;
//	Queue<int> q2_;
//};
//
//TEST_F(QueueTest, IsEmptyInitially) {
//	EXPECT_EQ(q0_.size(), 0);
//}
//
//TEST_F(QueueTest, DequeueWorks) {
//	int* n = q0_.Dequeue();
//	EXPECT_EQ(n, nullptr);
//
//	n = q1_.Dequeue();
//	ASSERT_NE(n, nullptr);
//	EXPECT_EQ(*n, 1);
//	EXPECT_EQ(q1_.size(), 0);
//
//	n = q2_.Dequeue();
//	ASSERT_NE(n, nullptr);
//	EXPECT_EQ(*n, 2);
//	EXPECT_EQ(q2_.size(), 1);
//}