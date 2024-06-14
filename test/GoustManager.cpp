#include "stdafx.h"
#include "Entry.h"
#include "GoustManager.h"

GoustManager& GoustManager::getInstance()
{
	static GoustManager _instance;
	return _instance;
}

void GoustManager::startAllChase()
{
	for (auto goust : m_goustSet)
	{
		if (goust)
			goust->setStatus(GoustStatus::en_Chase);
	}
}

void GoustManager::stopAllChase()
{
	for (auto goust : m_goustSet)
	{
		if (goust)
			goust->setStatus(GoustStatus::en_Sleep);
	}
}

void GoustManager::addGoust(Goust* ent)
{
	m_goustSet.push_back(ent);
}

void GoustManager::deleteGoust(Goust* ent)
{
	auto it = std::find(m_goustSet.begin(), m_goustSet.end(), ent);
	if (it != m_goustSet.end())
		m_goustSet.erase(it);
}

std::vector<Goust*> GoustManager::getAllGousts()
{
	return m_goustSet;
}
