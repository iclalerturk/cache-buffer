#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
	@brief dugumlerin yapisi
	@param sayfaAd sayfalarin adi
	@param sonraki sonraki dugumu isaret eder
	@param onceki onceki dugumu isaret eder
*/
typedef struct Node{
	char sayfaAd[20];
	int istek;
	struct Node *onceki;
	struct Node *sonraki;
}Node;
Node* basaSayfaEklemeYeniDugum(Node *, int *, char*);//basaSayfaEkle fonksiyonun icindeki tekrar eden bolum(yeni eklenen eleman dizide yoksa basa ekleme)
Node* basaSayfaEkle(char [20], Node* , int*, int);//basaSayfaEkle yeni girilen elemaný listeye ekleme
Node* sondanSayfaSil(Node*);//bufferin kapasitesi tastigi zaman sondaki elemani siler
void listeyiYazdir(Node *);//bufferi yazdirir
void dosyadanSayfaOku(char ***, char *, int *);//sayfa adlarini okur ve matrise atar
Node* hareket(Node*,int);//dugumler uzerinde saga sola gitmeyi saglar
Node* dugumSil(Node*,Node*);//bulundugu dugumu siler eger hareket edilmediyse head dugumunu siler
Node* dugumEkle(Node*,Node *, int );//bulundugu konumun soluna yeni dugum ekler eger hareket edelmediyse listenin basina ekler
Node* dugumdeGez(Node*);//dugumlerde gezinmeyi saglar
void bufferBosalt(Node *);//bufferý bosaltir
void matrisSerbestBirak(char **, int);//dosyadan okunan sayfa adreslerinin tutuldugu matrisi bosaltir
int main(){
	int secim, secimFlag=1,toplamSayac=0;//secim: string veya metinden okuma , secimFlag: secimin dogru yapilmasinin saglama, toplamSayac: yapýlan toplam istek sayisi
	int T=3, L=5, dugumSayisi=0;
	int boyut;//dosyadan okunan sayfa adi sayisini tutar
	char **matris;//dosyadan okunan sayfa adlarini tutar
	Node* head = NULL;
	printf("Esik Degeri: T=%d\n",T);
	printf("Kapasite: L=%d\n",L);
	printf("Sayfa adlarini okuma turunu seciniz.\n1.String olarak\n2.Metin belgesinden okuyarak\n");
	while(secimFlag){//gecerli secim yapýlana kadar tekrarlanýr
		printf("Secim: ");
		scanf("%d",&secim);
		if(secim!=1 && secim!=2){//yanlis secim tekrar secim sorulur
			printf("Lutfen gecerli secim yapiniz!\n");			
			secimFlag=1;
		}
		else{//dogru secim cikis
			secimFlag=0;
		}	
	}
	if(secim==1){//string okuma kismi
		char sayfa[20];
		int dd=1;//durma degiskeni
		printf("\nSayfa adlarini giriniz.(sayfa girdisini durdurmak icin 1 giriniz)");
		while(dd){//kullanici 1 girene kadar tekrar sorulur				
			printf("\nsayfa adi: ");
			scanf(" %s",sayfa);
			if(strcmp(sayfa,"1")==0){
				dd=0;
			}
			else{
				toplamSayac++;
				if(dugumSayisi<L){	//dugum sayisi kapasiteden azsa direkt eklenir			
					head = basaSayfaEkle(sayfa,head,&dugumSayisi,T);															
				}
				else if(dugumSayisi==L){//dugum sayisi kapasiteye ulasmissa sondaki dugum silinir
					head = basaSayfaEkle(sayfa,head,&dugumSayisi,T);
					if(dugumSayisi!=L)
						head =sondanSayfaSil(head);						
					dugumSayisi = L;	
				}
				listeyiYazdir(head);				
			}							
		}				
	}
	else{//belgeden okuma kismi		
		char fileName[20];
		printf("\nDosya adini giriniz: ");
		scanf(" %s",fileName);//sayfaAdres.txt
		dosyadanSayfaOku(&matris, fileName,&boyut);
		toplamSayac=boyut;
		int i;
		for(i=0;i<boyut;i++){//dosyadaki sayfa adi sayisi kadar tekrarlanir
			if(dugumSayisi<L)//dugum sayisi kapasiteden azsa direkt eklenir
				head = basaSayfaEkle(matris[i],head,&dugumSayisi,T);
			else{//dugum sayisi kapasiteye ulasmissa sondaki dugum silinir
				head = basaSayfaEkle(matris[i],head,&dugumSayisi,T);
				if(dugumSayisi!=L)
					head =sondanSayfaSil(head);						
				dugumSayisi = L;	
			}
			listeyiYazdir(head);
			printf("\n");		
		}		
	}
	int islemFlag=1, islem;
	Node *temp = head;
	while(islemFlag){
		printf("\nYapmak istediginiz islemi seciniz:\n");
		printf("1.Dugumler Uzerinde Gezinti\n");
		printf("2.Sayac Sorgulama\n");//toplam istek sayisi
		printf("3.Dugum Ekleme\n");
		printf("4.Dugum Silme\n");
		printf("5.Buffer'in temizlenmesi\n");
		printf("6.Cikis\n\n");
		printf("Secim: ");
		scanf("%d", &islem);
		if(islem==1){//dugumlerde gezinti
			listeyiYazdir(head);
			temp = dugumdeGez(head);
			if(temp==NULL)
				temp=head;			
		}
		else if(islem==2){//toplam istek sayisi
			printf("\nToplam istek sayisi: %d",toplamSayac);			
		}
		else if(islem==3){//dugum ekleme
			head = dugumEkle(temp,head,L);	
			temp = head;
			printf("\n");
			listeyiYazdir(head);
			printf("\n");
		}
		else if(islem==4){//dugum silme
			head = dugumSil(temp,head);
			temp = head;
			printf("\n");
			listeyiYazdir(head);
			printf("\n");	
		}	
		else if(islem==5){//bufferýn temizlenmesi
			if(secim==2){
				matrisSerbestBirak(matris, boyut);
			}
			bufferBosalt(head);				
		}
		else if(islem==6){//cikis
		printf("\nProgram sonlandirilimistir.\n");		
			islemFlag=0;
		}
		else{//gecerli giris yapilmadi
			printf("\nLutfen gecerli bir sayi giriniz!");
		}
	}	
	return 0;
}
//FONKSÝYONLAR
/*
@brief basaSayfaEkle fonksiyonun icindeki tekrar eden bolum(yeni eklenen eleman dizide yoksa basa ekleme)
@param head bufferýn basý
@param dugumSayisi bufferdaki dugum sayisi
@param sayfaAd buffera eklenecek sayfanin adi
@return head
*/
Node* basaSayfaEklemeYeniDugum(Node *head, int *dugumSayisi, char*sayfaAd){
	Node* yeniNode = (Node*)malloc(sizeof(Node));
		if (yeniNode == NULL) {
	        printf("\nSayfa olusturulamadi!\n");
		    return head;
		}  
		else{
			(*dugumSayisi)++;
			yeniNode->istek=1;
			strcpy(yeniNode->sayfaAd,sayfaAd);
			yeniNode->onceki=NULL;
			yeniNode->sonraki=head;
			if (head != NULL)
		        head->onceki = yeniNode;
			head = yeniNode;
			return head;	
		}
}
/*
@brief basaSayfaEkle yeni girilen elemaný listeye ekleme
@param head bufferýn basý
@param dugumSayisi bufferdaki dugum sayisi
@param sayfaAd buffera eklenecek sayfanin adi
@param T esik degeri
@return head
*/
Node* basaSayfaEkle(char sayfaAd[20], Node* head, int*dugumSayisi, int T){
	if(head != NULL){
		Node *suanki;
		suanki = head;
		while(suanki->sonraki!=NULL && strcmp(suanki->sayfaAd,sayfaAd)){
			suanki=suanki->sonraki;
		}		
		if(strcmp(suanki->sayfaAd,sayfaAd)==0){	// eklenecek sayfa bufferda varsa yeni eklemez ama esik degeri gecerse dugumu basa gecirir	
			suanki->istek=suanki->istek+1;//istek sayisi artirilir
			if(suanki->istek>T){//esik degeri gecerse headin degismesi
				if(suanki!=head){
					Node* yeniBas = (Node*)malloc(sizeof(Node));
					yeniBas->istek=suanki->istek;
					strcpy(yeniBas->sayfaAd,suanki->sayfaAd);
					yeniBas->onceki=NULL;
					yeniBas->sonraki=head;
					if(suanki->sonraki!=NULL){//NULL edurumuna gore farkli atamalar yapilir
						suanki->onceki->sonraki=suanki->sonraki;
						suanki->sonraki->onceki=suanki->onceki;
						head->onceki=yeniBas;
						return yeniBas;	
					}
					else{
						suanki->onceki->sonraki=NULL;
						head->onceki=yeniBas;
						head=yeniBas;
						return head;
					}							
				}										
			}		
			return head;										
		}
		else{//eklenecek dugum listede yoksa burasi calisir
			head = basaSayfaEklemeYeniDugum(head, dugumSayisi, sayfaAd);
			return head;		
		}			
	}
	else{//listede hic dugum yoksa burasi calisir
		head = basaSayfaEklemeYeniDugum(head, dugumSayisi, sayfaAd);
		return head;	
	}		
}
/*
@brief bufferin kapasitesi tastigi zaman sondaki elemani siler
@param head bufferýn basý
@return head
*/
Node* sondanSayfaSil(Node* head){
	if (head == NULL) {
	    printf("\nListe bos, silme islemi yapilamaz!\n");
	    return head;
	}
	Node *onceki, *suanki;
	onceki = head;
	suanki = head->sonraki;
	while(suanki->sonraki!=NULL){//son elemani bulma kismi
		onceki=suanki;
		suanki=suanki->sonraki;
	}	
	if (onceki != NULL)
        onceki->sonraki = NULL;
	return head;
}
/*
@brief bufferi yazdirir
@param head bufferýn basý
@return
*/
void listeyiYazdir(Node *head){
	Node *onceki, *suanki;
	onceki = head;
	suanki = head->sonraki;
	printf("%s,%d ",onceki->sayfaAd,onceki->istek);
	while(suanki!=NULL){
		printf("-><- %s,%d ",suanki->sayfaAd,suanki->istek);
		onceki=suanki;
		suanki=suanki->sonraki;
	}
}
/*
@brief sayfa adlarini okur ve matrise atar
@param matris dosyadan okunan sayfa adreslerini tutar
@param fileName okunacak dosyanin adi
@param boyut dosyadan okunan sayfa adresi sayisi
@param fp file pointer
@param temp dosyadan sayfa adreslerini okumak icin kullanilir 
@return
*/
void dosyadanSayfaOku(char ***matris, char *fileName, int *boyut){
	int i;
	char temp[20];
	FILE *fp = fopen(fileName,"r");
	if(!fp){
        printf("Dosya Acilamadi!");
        printf("Dosya adý: %s\n", fileName);
		return;
    }
    i=0;
    while(!feof(fp)){//dosyada yazan sayfa adresi sayisi
        fscanf(fp,"%s\n",temp);   
		i++; 
    }
	*boyut=i; 
	*matris = (char**)malloc((*boyut)*sizeof(char*));//boyut kadar yer acilir
	for(i=0;i<(*boyut);i++){
		(*matris)[i]=(char*)malloc(20*sizeof(char));
	}
	i=0;
	fseek(fp,0,SEEK_SET);//sayfanin basina gidilerek matrise sayfa adresleri atanir
	while(i<(*boyut)){
        fscanf(fp," %s\n",temp);  
		strcpy((*matris)[i],temp); 
		i++; 
    }    
  	fclose(fp); 
}
/*
@brief dugumler uzerinde saga sola gitmeyi saglar
@param head listenin basi
@param yon sag icin 1 sol icin 2
@param node listede gezinmeyi saglayan pointer
@return node hareket sonucu bulunan dugumun adresini dondurur
*/
Node* hareket(Node* head, int yon){
	Node *node;
	if(yon==1){//saga hareket
		node=head->sonraki;
	}
	else{//sola hareket
		node=head->onceki;
	}	
	if(node!=NULL){
		printf("\ngeldiginiz dugum: %s,%d\n",node->sayfaAd,node->istek);	
	}
	else{
		printf("\nListeden ciktiniz. Cikis yapildi.");//burdan sonra calismadigi icin cikis yapildi yazildi
	}	
	return node;
}
/*
@brief bulundugu dugumu siler eger hareket edilmediyse head dugumunu siler
@param head listenin basi
@param temp bulundugu dugumu isaret eder 
@return head
*/
Node* dugumSil(Node* temp,Node* head){
	if(temp!=NULL){
		if(temp->onceki!=NULL){
			if(temp->sonraki!=NULL){//tempin listenin ortasinda oludugu durumda burasi calisir
				temp->onceki->sonraki=temp->sonraki;
				temp->sonraki->onceki=temp->onceki;	
			}
			else{//temp listenin sonunda oldugu durumda burasi calisir
				temp->onceki->sonraki=NULL;
			}
			printf("\nDugum basarili bi sekilde silindi.Yeni liste:\n");	
			return head;
		}
		else{//temp listenin basinda ise head silinir
			temp->sonraki->onceki=NULL;
			printf("\nDugum basarili bi sekilde silindi.Yeni liste:\n");
			return temp->sonraki;//oncesi nullsa head degisir	
		}			
	}
	else{
		printf("\nBoyle bir dugum yok.\n");		
	}	
}
/*
@brief bulundugu konumun soluna yeni dugum ekler eger hareket edelmediyse listenin basina ekler
@param head listenin basi
@param temp bulundugu dugumu isaret eder
@param L buffer kapasitesi
@param dugumSayisi bufferdaki dugum sayisini hesaplar eger L degerini gecerse sondaki dugumu siler
@param sayfaAd eklenilecek sayfanin adresini 
@return head
*/
Node* dugumEkle(Node* temp,Node *head, int L){
	char sayfaAd[20];
	int dugumSayi=1;
	Node* node = (Node*)malloc(sizeof(Node));
	printf("\nEklemek istediginiz sayfanin adresini giriniz: ");
	scanf(" %s",sayfaAd);
	node->istek=1;
	strcpy(node->sayfaAd,sayfaAd);
	Node*suanki;
	suanki = head->sonraki;
	while(suanki!=NULL){//dugum sayisi hesaplanir
		dugumSayi++;
		suanki=suanki->sonraki;
	}
	if(dugumSayi==L){//dugum dayisi kapasiteye esitse yeni dugum eklenmeden sondaki silinir
		sondanSayfaSil(head);
	}
	if(temp!=NULL){
		if(temp->sonraki!=NULL){			
			if(temp->onceki!=NULL){	//listenin ortasina dugum ekleme
				temp->onceki->sonraki=node;
				node->sonraki=temp;
				node->onceki=temp->onceki;						
				temp->onceki=node;				
				return head;
			}
			else{//listenin basina dugum ekleme
				temp->onceki=node;
				node->sonraki=temp;
				node->onceki=NULL;				
				return node;//basa eklenirse head degisir	
			}			
		}
		else{
			if(temp->onceki!=NULL){	//lilstenin sonuna ekleme		
				temp->sonraki=node;
				node->onceki=temp;
				return head;				
			}
			else{//dugum yoksa yeni ekleme
				node->onceki=NULL;
				return node;//basa eklenirse head degisir	
			}
		}
	}	
}
/*
@brief dugumlerde gezinmeyi saglar
@param head listenin basi
@param temp bulundugu dugumu isaret eder ilk basta head dedir
@param flag baska islem secenegi secilene kadar ayni islemi tekrar etmesini saglar
@return temp bulundugu dugumun adresini dondurur
*/
Node* dugumdeGez(Node* head){	
	int secim;
	Node *temp=head;
	int flag=1;
	while(flag){
		printf("\nbulundugunuz dugum: %s,%d",temp->sayfaAd,temp->istek);
		printf("\ngitmek istediginiz yon(sag=1, sol=2, baska islem=3): ");
		scanf("%d",&secim);
		if(secim==1){//true
			temp = hareket(temp,1);
		}
		else if(secim==2){//true
			temp = hareket(temp,2);
		}
		else if(secim==3){//false
			flag=0;
		}
		else{//true
			printf("Gecerli giris yapiniz!");
		}	
	}
	return temp;	
}
/*
@brief bufferý bosaltir
@param head listenin basi
@param temp bulundugu dugumu isaret eder
@return
*/
void bufferBosalt(Node *head){
    Node *suanki = head;
    Node *temp;
    while (suanki != NULL) {//listenin sonuna kadar bosaltma islemini tekrar eder
        temp = suanki;
        suanki = suanki->sonraki;
        free(temp);
    }
    head = NULL;
    printf("\nBuffer bosaltildi.\n");
}
/*
@brief dosyadan okunan sayfa adreslerinin tutuldugu matrisi bosaltir
@param matris osyadan okunan sayfa adreslerinin tutuldugu matris
@param boyut osyadan okunan sayfa adreslerinin sayisi
@return
*/
void matrisSerbestBirak(char **matris, int boyut) {
    if (matris == NULL) return;
    int i;
    for (i = 0; i < boyut; i++) {
        free(matris[i]);
    }
    free(matris);
}
