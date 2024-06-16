#include "stdafx.h"
#include "Entry.h"
#include "ClashCheck.h"
#include "GoustManager.h"

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
			if (Player::getInstance()->isClash(*goust))
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
			if (move && still && move->isClash(*still))
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

