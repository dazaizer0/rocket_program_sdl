#pragma once

#include <vector>

namespace rnd {
	class elementary_object  {
	public:
		elementary_object() {

		}
	private:

	};

	class physics_object : public elementary_object {
	public:
		physics_object() {

		}
	private:

	};

	class physics_manager {
	public:
		std::vector<elementary_object> objects;
	private:
	
	};
}