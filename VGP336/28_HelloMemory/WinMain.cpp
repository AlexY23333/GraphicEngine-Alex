#include <SumEngine/Inc/SumEngine.h>

using namespace SumEngine;
using namespace SumEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
	Student()
	{
		mId = ++sUniqueId;
		mName = "NoName" + std::to_string(mId);
	}

	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{
	}

	void Log()
	{
		LOG("Name: %s - Id: %d", mName.c_str(), mId);
	}

private:
	int mId = 0;
	std::string mName;
};

static int sTeacherId = 0;
class Teacher
{
public:
	Teacher()
	{
		mId = ++sTeacherId;
		mName = "[REDACTED]" + std::to_string(mId);
	}

	Teacher(const std::string& name)
		: mName(name)
		, mId(++sTeacherId)
	{
	}

	void Log()
	{
		LOG("Name: %s - Id: %d", mName.c_str(), mId);
	}

private:
	int mId = 0;
	std::string mName;
};

class Weapon
{
public:
	Weapon()
	{
		mName = "Default_Weapon";
		mMaxAmmo = 30;
		mCurrentAmmo = mMaxAmmo;
	}

	Weapon(const std::string& name, int maxAmmo)
		: mName(name)
		, mMaxAmmo(maxAmmo)
		, mCurrentAmmo(maxAmmo)
	{
	}

	void Shoot()
	{
		if (mCurrentAmmo <= 0)
		{
			LOG("Ran out of ammo");
		}

		mCurrentAmmo--;
		LOG("Pew");
	}

private:
	std::string mName;
	int mMaxAmmo = 0;
	int mCurrentAmmo = 0;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
	std::vector<Student*> students;

#pragma region student
	// test new
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* student = studentPool.New();
		student->Log();
		students.push_back(student);
	}

	// test deletion
	for (uint32_t i = 0; i < 60; ++i)
	{
		Student* student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	// test adding more
	for (uint32_t i = 0; i < 55; ++i)
	{
		Student* student = studentPool.New("New_Student" + std::to_string(i));
		student->Log();
		students.push_back(student);
	}

	// clear all
	for (Student* student : students)
	{
		studentPool.Delete(student);
	}
	students.clear();
#pragma endregion

#pragma region teacher
	// Creating a small block and adding in more data to see its functionality

	TypedAllocator teacherPool = TypedAllocator<Teacher>("TeacherPool", 10);
	std::vector<Teacher*> teachers;

	for (uint32_t i = 0; i < 3; ++i)
	{
		Teacher* teacher = teacherPool.New();
		teacher->Log();
		teachers.push_back(teacher);
	}

	for (uint32_t i = 0; i < 1; ++i)
	{
		if (teachers.empty())
		{
			break;
		}

		Teacher* teacher = teachers.back();
		teacherPool.Delete(teacher);
		teachers.pop_back();
	}

	for (uint32_t i = 0; i < 100; ++i)
	{
		Teacher* teacher = teacherPool.New("New_Teacher" + std::to_string(i));
		if (teacher == nullptr)
		{
			break;
		}
		teacher->Log();
		teachers.push_back(teacher);
	}

	for (Teacher* teacher : teachers)
	{
		teacherPool.Delete(teacher);
	}
	teachers.clear();

#pragma endregion


#pragma region Weapon
	// Creating a small block and adding in more data to see its functionality

	TypedAllocator weaponPool = TypedAllocator<Weapon>("WeaponPool", 30);
	std::vector<Weapon*> weapons;

	for (uint32_t i = 0; i < 10; ++i)
	{
		Weapon* weapon = weaponPool.New();
		weapon->Shoot();
		weapons.push_back(weapon);
	}

	for (uint32_t i = 0; i < 2; ++i)
	{
		if (weapons.empty())
		{
			break;
		}

		Weapon* weapon = weapons.back();
		weaponPool.Delete(weapon);
		weapons.pop_back();
	}

	for (uint32_t i = 0; i < 20; ++i)
	{
		Weapon* weapon = weaponPool.New("New_Weapon" + std::to_string(i), 30);
		if (weapon == nullptr)
		{
			break;
		}
		weapon->Shoot();
		weapons.push_back(weapon);
	}

	for (Weapon* weapon : weapons)
	{
		weaponPool.Delete(weapon);
	}
	weapons.clear();

#pragma endregion

	return(0);
}