#include <stdio.h>
#include <string.h>

int main() {

	//+++++++++++++++++++++++++++++++++
    int tab[30000]{};
    
    // const char* code = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
    const char* code = ",>,>++++++++[<------<------>>-]<<[>[>+>+<<-]>>[<<+>>-]<<<-]>>>++++++[<++++++++>-]<.";
	const char* code_itr = code;
	const char* code_end = code;
    code_end += strlen(code);

	
	int* p = tab;

	int loop_delta_lvl = 0;

    char ch = 'c';

	while(code_itr<code_end) {
        ch = *code_itr;
		if(ch == '+') {
			++*p;
		}
		else if(ch == '-') {
			--*p;
		}
		else if(ch == '<') {
			--p;
		}
		else if(ch == '>') {
			++p;
		}
		else if(ch == '.') {
			// std::cout<<*p<<'\n';
			putchar(*p);
		}
		else if(ch == ',') {
			*p = getchar();
		}
		else if(ch == '[') {
			if(*p == 0) {
				loop_delta_lvl = 0;
				while(true) {
					++code_itr;
					if(*code_itr == ']') {
						if(loop_delta_lvl == 0) break;
						--loop_delta_lvl;
					}
					else if(*code_itr == '[') {
						++loop_delta_lvl;
					}
				}
			}
		}
		else if(ch == ']') {
			loop_delta_lvl = 0;
			while(true) {
				--code_itr;
				if(*code_itr == '[') {
					if(loop_delta_lvl == 0) {
						--code_itr;
						break;
					}
					--loop_delta_lvl;
				}
				else if(*code_itr == ']') {
					++loop_delta_lvl;
				}
			}
		}
		++code_itr;
	}
}