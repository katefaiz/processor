


CXXFLAGS := -std=c++11 -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat \
-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy \
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

PROCESSOR_OBJ := ./build/main_processor.o ./build/processor.o ./build/commands_funks.o ./build/assembler.o ./build/stack_funks.o ./build/stack_check.o

ASSEMBLER_OBJ := ./build/main_assembler.o ./build/assembler.o ./build/stack_funks.o ./build/stack_check.o

all: processor assembler

processor: $(PROCESSOR_OBJ)
	g++ $(CXXFLAGS) $(PROCESSOR_OBJ) -o ./build/processor


assembler: $(ASSEMBLER_OBJ)
	g++ $(CXXFLAGS) $(ASSEMBLER_OBJ) -o ./build/assembler


./build/main_processor.o: ./processor/source/main_processor.cpp
	g++ $(CXXFLAGS) -c -I./processor/include -I./stack/include -I./assembler/include $< -o $@

./build/processor.o: ./processor/source/processor.cpp
	g++ $(CXXFLAGS) -c -I./processor/include -I./stack/include -I./assembler/include $< -o $@

./build/commands_funks.o: ./processor/source/commands_funks.cpp
	g++ $(CXXFLAGS) -c -I./processor/include -I./stack/include -I./assembler/include $< -o $@

./build/main_assembler.o: ./assembler/source/main_assembler.cpp
	g++ $(CXXFLAGS) -c -I./assembler/include -I./stack/include $< -o $@

./build/assembler.o: ./assembler/source/assembler.cpp
	g++ $(CXXFLAGS) -c -I./assembler/include -I./stack/include -I./processor/include $< -o $@

./build/stack_funks.o: ./stack/source/stack_funks.cpp
	g++ $(CXXFLAGS) -c -I./stack/include $< -o $@

./build/stack_check.o: ./stack/source/stack_check.cpp
	g++ $(CXXFLAGS) -c -I./stack/include $< -o $@

clean:
	rm -rf ./build/*.o ./build/processor ./build/assembler

# CXXFLAGS := -std=c++11 -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat \
# -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy \
# -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
# -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
# -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
# -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
# -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
# -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
# -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
# -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# CPPOBJ :=  ./stack_funks.o ./stack_check.o ./processor.o ./commands_funks.o ./assembler.o

# processor.exe: $(CPPOBJ
# 	g++ $(CXXFLAGS) $(CPPOBJ) -o ./processor.exe 

# # ./main.o: ./main.cpp
# # 	g++ $(CXXFLAGS) -c ./main.cpp -o ./main.o

# ./stack_funks.o: ./stack_funks.cpp
# 	g++ $(CXXFLAGS) -c ./stack_funks.cpp -o ./stack_funks.o

# ./stack_check.o: ./stack_check.cpp
# 	g++ $(CXXFLAGS) -c ./stack_check.cpp -o ./stack_check.o

# ./processor.o: ./processor.cpp
# 	g++ $(CXXFLAGS) -c ./processor.cpp -o ./processor.o

# ./commands_funks.o: ./commands_funks.cpp
# 	g++ $(CXXFLAGS) -c ./commands_funks.cpp -o ./commands_funks.o

# ./assembler.o: ./assembler.cpp
# 	g++ $(CXXFLAGS) -c ./assembler.cpp -o ./assembler.o


# processor: $(CPPOBJ)
# 	g++ $(CXXFLAGS) $(CPPOBJ) -o ./processor

# clean:
# 	rm -rf ./*.o ./*.exe