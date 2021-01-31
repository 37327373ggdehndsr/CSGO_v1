#pragma once

class c_base_handle {
public:
	c_base_handle() { m_index = INVALID_EHANDLE_INDEX; }
	c_base_handle(const c_base_handle& other) { m_index = other.m_index; }
	c_base_handle(unsigned long value) { m_index = value; }
	c_base_handle(int entry, int serial_num) { init(entry, serial_num); }

	void init(int entry, int serial_num) { m_index = (unsigned long)(entry | (serial_num << NUM_SERIAL_NUM_SHIFT_BITS)); }
	void term() { m_index = INVALID_EHANDLE_INDEX; }

	bool is_valid() const { return m_index != INVALID_EHANDLE_INDEX; }

	int get_entry_index() const {
		if (!is_valid())
			return NUM_ENT_ENTRIES - 1;

		return m_index & ENT_ENTRY_MASK;
	}

	int get_serial_number() const { return m_index >> NUM_SERIAL_NUM_SHIFT_BITS; }

	int to_int() const { return (int)m_index; }

	bool operator !=(const c_base_handle& other) const { return m_index != other.m_index; }

	bool operator ==(const c_base_handle& other) const { return m_index == other.m_index; }

	bool operator ==(const i_handle_entity* entity) const { return get() == entity; };

	bool operator !=(const i_handle_entity* entity) const { return get() != entity; }

	bool operator <(const c_base_handle& other) const { return m_index < other.m_index; }

	bool operator <(const i_handle_entity* entity) const {
		unsigned long other_index = entity ? entity->get_handle().m_index : INVALID_EHANDLE_INDEX;
		return m_index < other_index;
	}

	const c_base_handle& operator=(const i_handle_entity* entity) { return set(entity); }

	const c_base_handle& set(const i_handle_entity* entity) {
		if (entity)
			*this = entity->get_handle();
		else
			m_index = INVALID_EHANDLE_INDEX;

		return *this;
	}

	i_handle_entity* get() const;
protected:
	unsigned long  m_index;
};