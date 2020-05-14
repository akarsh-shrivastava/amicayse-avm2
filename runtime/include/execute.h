#ifndef EXECUTE_H_DEFINED
    #define EXECUTE_H_DEFINED

	enum {
		LOG_IP = 0x1,
		LOG_DS = 0x2,
		LOG_SS = 0x4,
		LOG_CS = 0x8
	};
	void execute(char flags);

#endif