#pragma once
#include "Observer.h"
#include <vector>

class Notifier
{
public:
	Notifier() {}
	~Notifier() { m_observers.clear(); }

	bool AddObserver(Observer* l_obs)
	{
		auto itr = m_observers.begin();
		while (itr != m_observers.end())
		{
			if (*itr == l_obs)
				return false;// Observer already exists, cannot add
			itr++;
		}

		m_observers.emplace_back(l_obs);
		return true;
	}

	bool HasObserver(Observer* l_obs)
	{
		auto itr = m_observers.begin();
		while (itr != m_observers.end())
		{
			if (*itr == l_obs)
				return true;
			itr++;
		}
		return false; // Observer does not exist
	}

	bool RemoverObserver(Observer* l_obs)
	{
		auto itr = m_observers.begin();
		while (itr != m_observers.end())
		{
			if (*itr == l_obs)
			{
				m_observers.erase(itr);
				return true;
			}
			itr++;
		}
		return false; // Observer does not exist, cannot be removed
	}

	void Broadcast(const Message& l_msg)
	{
		for (auto itr : m_observers)
			itr->Notify(l_msg);
	}
private:
	std::vector<Observer*> m_observers;
};