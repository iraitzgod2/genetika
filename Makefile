
# 'genetika' proiekturako erabiliko den Makefile fitxategia
# *********************************************************
# Proiektu- eta konpilazio-aldagaiak:

# Paraleloan konpilatzeko
IN_FILES=taldegen_p.c funtg_p.c
OUT_FILE=taldegen_p

# Seriean konpilatzeko
IN_SERIE_FILES=taldegen_s.c funtg_s.c
OUT_SERIE_FILE=taldegen_s

# Beharrezko flag-ak
FLAGS=-O2 -lm
# Paraleloan konpilatzeko flag-ak, OpenMP bidez
PFLAGS=-fopenmp

# Defektuz paraleloan konpilatuko da:
all: paral

# Paraleloko eta serieko konpilazioa:
paral:
	gcc $(FLAGS) $(PFLAGS) $(IN_FILES) -o $(OUT_FILE)
serie:
	gcc $(FLAGS) $(IN_SERIE_FILES) -o $(OUT_SERIE_FILE)

# Clean defektuz serie nahiz paraleloko fitxategiak ezabatzen ditu
clean: sclean pclean
	
sclean:
	rm $(OUT_SERIE_FILE)
pclean:
	rm $(OUT_FILE)
