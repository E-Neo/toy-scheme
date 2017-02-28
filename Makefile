OCB_FLAGS = -I src
OCB = ocamlbuild $(OCB_FLAGS)
native:
	$(OCB) calc.native

clean:
	$(OCB) -clean
