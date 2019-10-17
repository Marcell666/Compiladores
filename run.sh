echo "generating files"
make
echo "done"
mkdir saidas
rm -f saidas/saidaErro;
for f in "testes/"*;
	do	rm -f "saidas/saida_"${f#testes/};
		echo "--->Teste do arquivo ${f#testes/}";
		echo "--->Teste do arquivo ${f#testes/}" >> saidas/saidaErro;
		#./prog < $f >> "saidas/saida_"${f#testes/} 2>> saidas/saidaErro;
		./prog < $f &>> "saidas/saida_"${f#testes/};
		#./prog < $f >> "saidas/saida_"${f#testes/};
		#./prog < $f;
		echo "" >> "saidas/saida_"${f#testes/}
		echo "" >> saidas/saidaErro
	done;


#rm -f "saidas/saida_keyword";
#echo "--->Teste do arquivo keyword" >> saidas/saidaErro
#./prog < testes/keyword
#./prog < $f;
#echo "" >> "saidas/saida_keyword"
#echo "" >> saidas/saidaErro
