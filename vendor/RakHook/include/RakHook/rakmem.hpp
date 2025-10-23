#ifndef RAKMEM_HPP
#define RAKMEM_HPP


namespace mem {
	class unprot {
	public:
		unprot(std::uintptr_t addr, std::size_t size);
		unprot(void* ptr, std::size_t size);
		~unprot();

	private:
		std::uintptr_t addr_ = 0;
		std::size_t	   size_ = 0;

		DWORD old_prot_ = 0;
	};

	inline unprot::unprot(std::uintptr_t addr, std::size_t size)
		: addr_(addr), size_(size) {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, PAGE_EXECUTE_READWRITE, &old_prot_);
	}

	inline unprot::unprot(void* ptr, std::size_t size)
		: addr_(reinterpret_cast<std::uintptr_t>(ptr)), size_(size) {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, PAGE_EXECUTE_READWRITE, &old_prot_);
	}

	inline unprot::~unprot() {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, old_prot_, nullptr);
	}

	inline void safe_copy(void* dst, const void* src, std::size_t size) {
		unprot unprot(dst, size);
		std::memcpy(dst, src, size);
	}

	inline void safe_copy(std::uintptr_t dst, std::uintptr_t src, std::size_t size) {
		safe_copy(reinterpret_cast<void*>(dst), reinterpret_cast<void*>(src), size);
	}

	inline void safe_set(void* dst, std::uint8_t value, std::size_t size) {
		unprot unprot(dst, size);
		std::memset(dst, value, size);
	}

	inline void safe_set(std::uintptr_t dst, std::uint8_t value, std::size_t size) {
		safe_set(reinterpret_cast<void*>(dst), value, size);
	}

	inline bool compare_bytes(const std::uint8_t* data, const std::uint8_t* bytes, const char* mask) {
		if (!data || !bytes || !mask) return false;
		for (; *mask; ++mask, ++data, ++bytes) {
			if (*mask == 'x' && std::memcmp(data, bytes, 1) != 0) {
				return false;
			}
		}
		return *mask == 0;
	}

	inline bool compare_bytes(const std::uint8_t* data, const char* bytes, const char* mask) {
		return compare_bytes(data, reinterpret_cast<const std::uint8_t*>(bytes), mask);
	}

	inline std::uintptr_t find_pattern(std::uintptr_t base, std::size_t len, const std::uint8_t* bytes, const char* mask) {
		for (auto i = 0u; i < len; ++i) {
			if (compare_bytes(reinterpret_cast<std::uint8_t*>(base + i), bytes, mask)) {
				return base + i;
			}
		}
		return 0;
	}

	inline std::uintptr_t find_pattern(std::uintptr_t base, std::size_t len, const char* bytes, const char* mask) {
		return find_pattern(base, len, reinterpret_cast<const std::uint8_t*>(bytes), mask);
	}

}


#endif // RAKMEM_HPP