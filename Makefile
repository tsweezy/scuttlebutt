all:
	clang++ -o scuttlebutt *.cpp 

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)