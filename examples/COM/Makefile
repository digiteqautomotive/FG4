EXE = fg4.exe
HDR = fg4.h
IDL = fg4.idl
OBJ = fg4.obj

all: $(EXE)

$(HDR):
	midl /h $(HDR) $(IDL)

$(EXE): $(HDR) $(OBJ)
	link -out:$@ $(OBJ) strmiids.lib ole32.lib

.cpp.obj:
	cl -c $<

clean:
	del *.h *.c *.obj
	del $(EXE)
