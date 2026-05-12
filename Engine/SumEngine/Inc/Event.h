#pragma once

namespace SumEngine
{
	enum class EventType
	{
		None,
		SpacePressed,
		MKeyPressed,
		AnimationKey
	};

	class Event
	{
	public:
		Event() = default;
		Event(EventType et) : mEventType(et) {}
		virtual ~Event() = default;

		EventType GetType() const { return mEventType; }

	private:
		EventType mEventType = EventType::None;
	};

	class SpacePressedEvent : public Event
	{
	public:
		SpacePressedEvent() : Event(EventType::SpacePressed) {}
	};

	class MKeyPressedEvent : public Event
	{
	public:
		MKeyPressedEvent() : Event(EventType::MKeyPressed) {}
	};

	class AnimationKeyEvent : public Event
	{
	public:
		AnimationKeyEvent() : Event(EventType::AnimationKey) {}
		AnimationKeyEvent(int index) : Event(EventType::AnimationKey), mIndex(index) {}
		int GetIndex() const { return mIndex; }
		void SetIndex(int index) { mIndex = index; }

	private:
		int mIndex = 0;
	};
}
