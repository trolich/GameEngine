#include "EventManager.h"
#include "Utilities.h"

EventManager::EventManager():m_currentState(GameStateType(0)),m_hasFocus(true)
{
	std::string directory = Utils::GetWorkingDirectory() + "keyConfig.cfg";
	LoadBindings(directory);
}

EventManager::~EventManager()
{
	for (auto &itr : m_bindings)
		delete itr.second;
}

bool EventManager::AddBinding(Binding* l_bind)
{
	if (m_bindings.find(l_bind->m_name) != m_bindings.end())
		return false;
	return m_bindings.emplace(l_bind->m_name, l_bind).second;
}

bool EventManager::RemoveBinding(std::string& l_name)
{
	auto itr = m_bindings.find(l_name);
	if (itr == m_bindings.end())
		return false;

	delete itr->second;
	m_bindings.erase(itr);
	return true;
}

void EventManager::SetGameState(const GameStateType& l_type)
{
	m_currentState = l_type;
}

void EventManager::SetFocus(bool l_focus)
{
	m_hasFocus = l_focus;
}

// Handle Event iterates through the events of all the bindings searching for an event which matches the parameter
// Once it finds the right event it will check and see if it is a key, a mouse button, or something else
// Depending on which type it is it will set up the details of the binding with the information from the event
void EventManager::HandleEvent(sf::Event& l_event)
{
	for (auto &bind_itr : m_bindings) // Cycle through bindings
	{
		Binding* bind = bind_itr.second;
		for (auto &event_itr : bind->m_events) // Cycle through events of each binding
		{
			EventType currentEvent = (EventType)l_event.type;
			if (event_itr.first != currentEvent)
				continue; // Doesn't match, check next

			if (currentEvent == EventType::KeyPressed || currentEvent == EventType::KeyRelease)
			{
				if (event_itr.second.m_code == l_event.key.code)
				{
					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = event_itr.second.m_code;
				}
				++(bind->c);
				break;
			}
			else if (currentEvent == EventType::MouseButtonDown || currentEvent == EventType::MouseButtonUp)
			{
				if (event_itr.second.m_code == l_event.mouseButton.button)
				{
					bind->m_details.m_mouse.x = l_event.mouseButton.x;
					bind->m_details.m_mouse.y = l_event.mouseButton.y;

					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = event_itr.second.m_code;
				}
				++(bind->c);
				break;
			}
			else
			{
				if (currentEvent == EventType::MouseWheel)
				{
					bind->m_details.m_mouseWheelDelta = l_event.mouseWheel.delta;
				}
				else if (currentEvent == EventType::WindowResized)
				{
					bind->m_details.m_size.x = l_event.size.width;
					bind->m_details.m_size.y = l_event.size.height;
				}
				else if (currentEvent == EventType::TextEntered)
				{
					bind->m_details.m_textEntered = l_event.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::Update()
{
	if (!m_hasFocus) //No need to worry about updating
		return;

	for (auto &bind_itr : m_bindings) // Cycle through bindings
	{
		Binding* bind = bind_itr.second;
		for (auto &event_itr : bind->m_events) // Cycle through events of each binding
		{
			switch (event_itr.first)
			{
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(event_itr.second.m_code)))
				{
					if(bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = event_itr.second.m_code;

					++(bind->c);
				}
				break;
			case(EventType::Mouse):
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(event_itr.second.m_code)))
				{
					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = event_itr.second.m_code;

					++(bind->c);
				}
				break;
			case(EventType::Joystick):
				break;
			}
		}

		if (bind->m_events.size() == bind->c)
		{
			auto stateCallbacks = m_callbacks.find(m_currentState);
			auto otherCallbacks = m_callbacks.find(GameStateType(0));

			if (stateCallbacks != m_callbacks.end())
			{
				auto callItr = stateCallbacks->second.find(bind->m_name);
				if (callItr != stateCallbacks->second.end())
					callItr->second(&bind->m_details);
			}
			if (otherCallbacks != m_callbacks.end())
			{
				auto callItr = otherCallbacks->second.find(bind->m_name);
				if (callItr != otherCallbacks->second.end())
					callItr->second(&bind->m_details);
			}
		}
		bind->c = 0;
		bind->m_details.Clear();
	}
}

void EventManager::LoadBindings(std::string& l_dir)
{
	std::string delimiter = ":";

	std::ifstream bindFile;
	bindFile.open(l_dir);
	if (!bindFile.is_open())
	{
		std::cout << "Failed to load binding file from " << l_dir << std::endl; return;
	}

	std::string line;
	while (std::getline(bindFile, line))
	{
		std::stringstream keyStream(line);
		std::string callbackName;
		keyStream >> callbackName;
		Binding* bind = new Binding(callbackName);

		while (!keyStream.eof())
		{
			std::string keyval;
			keyStream >> keyval;

			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos) { delete bind; bind = nullptr; break; }

			EventType eventType = EventType(std::stoi(keyval.substr(start, end - start)));

			EventInfo eventInfo;
			int code = std::stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			eventInfo.m_code = code;

			bind->BindEvent(eventType, eventInfo);
		}
		
		if (!AddBinding(bind))
		{
			std::cout << "Problem adding the binding" << std::endl;
			delete bind;
		}
		bind = nullptr;
	}
	bindFile.close();
}

void EventManager::LoadBindings()
{
	std::string directory = "This is the Directory";
	LoadBindings(directory);
}