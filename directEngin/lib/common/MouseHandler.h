#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include <bitset>
#include "../vec2.h"

class MouseHandler
{
	friend class Window;
public:
	class Event {

	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid,
		};
	private:
		Type _type;
		bool _leftIsPressed;
		bool _rightIsPressed;
		int _x, _y;
	public:
		Event() :_type(Type::Invalid), _leftIsPressed(false),_rightIsPressed(false),_x(0),_y(0) {};
		Event(Type type, MouseHandler& parent) noexcept :_type(type), _leftIsPressed(parent._leftIsPressed),_rightIsPressed(parent._rightIsPressed),_x(parent._x),_y(parent._y) {};
		

		bool leftIsPressed() const noexcept {
			return _leftIsPressed;
		}

		bool rightIsPressed() const noexcept {
			return _rightIsPressed;
		}
		bool isValid() const noexcept {

			return _type != Type::Invalid;
		}

		Type getType() const noexcept {
			return _type;
;		}
		Vec2 getPos() const noexcept {
			return Vec2(_x, _y);
		}

		int getPosX() const noexcept {
			return _x;
		}

		int getPosY() const noexcept {
			return _y;
		}
	};
public:

	MouseHandler() = default;
	MouseHandler(const MouseHandler&) = delete;
	MouseHandler& operator=(const MouseHandler&) = delete;

	Vec2 getPos() const noexcept;
	int getPosX() const noexcept;
	int getPosY() const noexcept;
	bool leftIsPressed() const noexcept;
	bool rightIsPressed() const noexcept;
	MouseHandler::Event read() noexcept;
	bool isEmpty() const noexcept {
		return _buffer.empty();
	}
	void flush() noexcept;
private:
	void trimBuffer() noexcept;
	void onMouseMove(int x, int y) noexcept;

	void onLeftPressed(int x, int y) noexcept;
	void onLeftReleased(int x, int y) noexcept;

	void onRightPressed(int x, int y) noexcept;
	void onRightReleased(int x, int y) noexcept;

	void onWheelUp(int x, int y) noexcept;
	void onWheelDown(int x, int y) noexcept;


	static constexpr unsigned int _bufferSize = 16u;

	bool _leftIsPressed;
	bool _rightIsPressed;
	int _x, _y;
	std::queue<Event> _buffer;

};

