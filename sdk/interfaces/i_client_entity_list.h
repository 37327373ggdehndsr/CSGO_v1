#pragma once

class i_client_entity_list {
public:
	virtual i_client_networkable*   get_client_networkable(int index) = 0;
	virtual void*					unknown0() = 0;
	virtual void*					unknown1() = 0;
	virtual i_client_entity*		get_client_entity(int index) = 0;
	virtual i_client_entity*		get_client_entity_from_handle(c_base_handle handle) = 0;
	virtual int						number_of_entities(bool include_non_networkable) = 0;
	virtual int						get_highest_entity_index() = 0;
	virtual void					set_max_entities(int num) = 0;
	virtual int						get_max_entities() = 0;
};