#include "stdafx.h"
#include "Entry.h"
#include "ClashCheck.h"
#include "GoustManager.h"


static bool isClash(const Entry* ent1, const Entry* ent2)
{
	int left1 = ent1->m_pos.x;
	int right1 = left1 + ent1->m_size.x - 1;
	int top1 = ent1->m_pos.y;
	int bottom1 = top1 + ent1->m_size.y - 1;

	int left2 = ent2->m_pos.x;
	int right2 = left2 + ent2->m_size.x - 1;
	int top2 = ent2->m_pos.y;
	int bottom2 = top2 + ent2->m_size.y - 1;

	if (left1 > right2 || left2 > right1 || top1 > bottom2 || top2 > bottom1)
		return false;
	return true;
}

ClashCheckManager& ClashCheckManager::getInstance()
{
	static ClashCheckManager _instance;
	return _instance;
}

void ClashCheckManager::clashCheck()
{
	for (auto goust : GoustManager::getInstance().getAllGousts())
	{
		if (goust)
		{
			if (isClash(Player::getInstance(), goust))
			{
				Player::getInstance()->_Clash(*goust);
				goust->_Clash(*Player::getInstance());
			}
			else if (goust->getStatus() == GoustStatus::en_Chase)
				goust->Chase();
		}
	}

	for (auto move : m_movingEntry)
	{
		for (auto still : m_stillEntry)
		{
			if (move && still && isClash(move, still))
			{
				move->_Clash(*still);
			}
		}
	}
}

void ClashCheckManager::addMovingEntry(Entry* ent)
{
	m_movingEntry.push_back(ent);
}

void ClashCheckManager::deleteMovingEntry(Entry* ent)
{
	auto it = std::find(m_movingEntry.begin(), m_movingEntry.end(), ent);
	if (it != m_movingEntry.end())
		m_movingEntry.erase(it);
}

void ClashCheckManager::addStillEntry(Entry* ent)
{
	m_stillEntry.push_back(ent);
}

void ClashCheckManager::deleteStillEntry(Entry* ent)
{
	auto it = std::find(m_stillEntry.begin(), m_stillEntry.end(), ent);
	if (it != m_stillEntry.end())
		m_stillEntry.erase(it);
}

