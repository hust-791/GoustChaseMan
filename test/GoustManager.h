#pragma once

class GoustManager
{
public:
	static GoustManager& getInstance();

	void startAllChase();

	void stopAllChase();

	void addGoust(Goust* ent);

	void deleteGoust(Goust* ent);

	GoustManager(const GoustManager&) = delete;
	GoustManager& operator=(const GoustManager&) = delete;

private:
	GoustManager() {}
	~GoustManager() {};

	std::vector<Goust*> m_goustSet;
};