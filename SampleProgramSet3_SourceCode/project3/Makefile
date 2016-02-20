CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = project3.o ModelViewWithPhongLighting.o Floor.o Table.o Umbrella.o Glass.o Napkin.o Chair.o Block.o Cylinder.o Bowl.o HalfSphere.o Sphere.o DogToy.o Controller_Additions.o ModelView_Additions.o

project3: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -o project3 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project3.o: project3.c++
	$(CPP) $(C_FLAGS) project3.c++
Floor.o: Floor.h Floor.c++
	$(CPP) $(C_FLAGS) Floor.c++
Table.o: Table.h Table.c++
	$(CPP) $(C_FLAGS) Table.c++
Umbrella.o: Umbrella.h Umbrella.c++
	$(CPP) $(C_FLAGS) Umbrella.c++
Glass.o: Glass.h Glass.c++
	$(CPP) $(C_FLAGS) Glass.c++
Napkin.o: Napkin.h Napkin.c++
	$(CPP) $(C_FLAGS) Napkin.c++
Chair.o: Chair.h Chair.c++
	$(CPP) $(C_FLAGS) Chair.c++
Block.o: Block.h Block.c++
	$(CPP) $(C_FLAGS) Block.c++
Cylinder.o: Cylinder.h Cylinder.c++
	$(CPP) $(C_FLAGS) Cylinder.c++
Bowl.o: Bowl.h Bowl.c++
	$(CPP) $(C_FLAGS) Bowl.c++
HalfSphere.o: HalfSphere.h HalfSphere.c++
	$(CPP) $(C_FLAGS) HalfSphere.c++
Sphere.o: Sphere.h Sphere.c++
	$(CPP) $(C_FLAGS) Sphere.c++
DogToy.o: DogToy.h DogToy.c++
	$(CPP) $(C_FLAGS) DogToy.c++
Controller_Additions.o: Controller_Additions.c++
	$(CPP) $(C_FLAGS) Controller_Additions.c++
ModelView_Additions.o: ModelView_Additions.c++
	$(CPP) $(C_FLAGS) ModelView_Additions.c++
ModelViewWithPhongLighting.o: ModelViewWithPhongLighting.h ModelViewWithPhongLighting.c++
	$(CPP) $(C_FLAGS) ModelViewWithPhongLighting.c++

clean:
	rm project3 *.o
