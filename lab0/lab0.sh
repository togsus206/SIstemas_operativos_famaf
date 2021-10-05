#ejercicio 1

grep name /proc/cpuinfo


#ejercicio 2

grep name /proc/cpuinfo | wc -l


#ejercicio 3

wget https://www.gutenberg.org/files/11/11-0.txt && cat 11-0.txt > MATIAS_in_wonderland.txt | sed -i "s/Alice/Matias/g" MATIAS_in_wonderland.txt && rm 11-0.txt

#ejercicio 4

sort -nrk 5 weather_cordoba.in && echo -e "\n" && sort -nrk 5 weather_cordoba.in | head -1 | awk '{ print $1,$2,$3 }' && sort -nrk 5 weather_cordoba.in | tail -1 | awk '{ print $1,$2,$3 }'

#ejercicio 5

sort -nk 3 atpplayers.in 

#ejercicio 6 

awk '{print $0,$7-$8}' superliga.in | sort -nrk2,2 -nrk9,9

#ejercicio 7 

ip a | grep -i link/ether

#ejercicio 8 

mkdir aventuras_en_el_lago_ness && cd aventuras_en_el_lago_ness && touch fma_S01E{1..10}_es.srt && rename 's/_es//' *.srt #forma todos los nombre iguales

mkdir aventuras_en_el_lago_ness && cd aventuras_en_el_lago_ness && for i in "El_principio_final_es" "Claro_como_el_agua_es" "Encuentro_con_el_es" "se_complica_el_viaje_es" "Perdidos_es" "estelar_encuentro_es" "ni_tan_mal_es" "el_fin_del_viaje?_es" "el_eden_es" "la_despedida_es"; do touch $i.srt; done && rename 's/_es//' *.srt
#forma: cada capitulo nombre diferente