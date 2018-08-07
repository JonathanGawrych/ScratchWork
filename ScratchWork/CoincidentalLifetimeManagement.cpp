#include "ScratchPicker.h"
#include "CallBadCode.h"

#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

struct Baz
{
	Baz(int i) : m_i(i) {}

	int m_i;
};

// Bar owns a reference to Baz that's managed by Coincidental Lifetime Management (TM)
// Just because its pared with Baz inside of Foo, doesn't mean code changes can cause major problems
struct Bar
{
	Bar(Baz* pBaz) : m_pBaz(pBaz) {}

	Baz* m_pBaz;

	// Instead do:
	// std::shared_ptr<Baz> m_spBaz;
};

struct Foo
{
	Foo(std::shared_ptr<Bar> spBar, std::shared_ptr<Baz> spBaz) : m_spBar(spBar), m_spBaz(spBaz) {}

	std::shared_ptr<Bar> m_spBar;
	std::shared_ptr<Baz> m_spBaz;
};

std::shared_ptr<Foo> make_foo()
{
	// Create a baz that has a value
	std::shared_ptr<Baz> spBaz = std::make_shared<Baz>(42);

	// Create a bar that knows about baz, but doesn't own it
	std::shared_ptr<Bar> spBar = std::make_shared<Bar>(spBaz.get());

	// Create a foo that owns bar and baz
	return std::make_shared<Foo>(spBar, spBaz);
}

void fragileButWorksLValues()
{
	// Get the foo
	std::shared_ptr<Foo> spFoo = make_foo();

	// Get its bar
	std::shared_ptr<Bar> spBar = spFoo->m_spBar;

	// Get its baz
	Baz* pBaz = spBar->m_pBaz;

	// print out its member, value 42
	std::cout << pBaz->m_i << std::endl;

	// ~Foo called here!
	// ~Bar called here!
	// ~Baz called here!
}

void doesntWork()
{
	// Get a bar
	std::shared_ptr<Bar> spBar = make_foo()->m_spBar;
	// ~Foo called here!
	// ~Baz called here!

	// Get its baz
	Baz* pBaz = spBar->m_pBaz;

	// Print out its member, value 42
	// Garbage pointer dereference.
	std::cout << pBaz->m_i << std::endl;

	// ~Bar called here! (if we didn't crash on the above call)
}

void fragileButWorksRValues()
{
	// print out its member, value 42
	std::cout << make_foo()->m_spBar->m_pBaz->m_i << std::endl;
	// ~Foo called here!
	// ~Bar called here!
	// ~Baz called here!
}

void aFooAndABazWalkIntoABar()
{
	fragileButWorksLValues();
	
	callBadCodeAndIgnoreDeadlyErrors(doesntWork);

	fragileButWorksRValues();
}

struct Sonic
{
	Sonic(int i) : m_i(i)
	{
	}

	void GoFastAsync()
	{
		std::thread([this]()
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);

			std::cout << "Sonic finish: " << m_i << std::endl;
		}).detach();

		std::cout << "Sonic start" << std::endl;
	}

	int m_i;
};

struct Tails : std::enable_shared_from_this<Tails>
{
	Tails(int i) : m_i(i)
	{
	}

	void GoSafeAsync()
	{
		std::shared_ptr<Tails> spThis = shared_from_this();
		std::thread([spThis]()
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);

			std::cout << "Tails finish: " << spThis->m_i << std::endl;
		}).detach();

		std::cout << "Tails start" << std::endl;
	}

	int m_i;
};

void race()
{
	std::shared_ptr<Sonic> sonic = std::make_shared<Sonic>(1);
	std::shared_ptr<Tails> tails = std::make_shared<Tails>(2);
	sonic->GoFastAsync();
	tails->GoSafeAsync();
}

template<>
void RunScratch<ScratchWork::CoincidentalLifetimeManagement>()
{
	aFooAndABazWalkIntoABar();
	race();
}
