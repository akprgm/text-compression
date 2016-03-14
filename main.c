#include <stdio.h>
#include <stdlib.h>
#include<math.h>
struct PQ{//structure for priority queue
    int frequency;
    struct cTree *cTree;
    struct PQ *next;
};
struct cTree{//structure for cTree which is going to formed in huffman algorithm
    void *data;
    int *binaryCode;
    struct cTree *leftNode;
    struct cTree *rightNode;
};
int *frequency;//point to the address of frequency array which contain the ASCII Characters frequency
int *codeLength;//point to the address of frequency array which contain code length assigned to particular character
int *codewords;
/*
    #Function for creating priority queue based on the frequency of characters
*/
struct PQ *insertInPQ(struct PQ **startPQ,int frequency,int data){//this function is for inserting node in priority queue
    if(!(*startPQ)){
        struct PQ *newNode=(struct PQ *)malloc(sizeof(struct PQ));
        newNode->frequency=frequency;
        newNode->next=NULL;
        newNode->cTree=(struct cTree*)malloc(sizeof(struct cTree));
        (newNode->cTree)->data=(void *)data;
        (newNode->cTree)->binaryCode=NULL;
        (newNode->cTree)->leftNode=NULL;
        (newNode->cTree)->rightNode=NULL;
        *startPQ=newNode;
    }else{
        if((*startPQ)->frequency<frequency){
            struct PQ *temp1=NULL,*temp2=*startPQ;
            while(temp2->frequency < frequency){//loop for getting the position where we have to insert new Node
                temp1=temp2;
                temp2=temp2->next;
                if(!temp2)
                    break;
            }
            struct PQ *newNode=(struct PQ *)malloc(sizeof(struct PQ));
            temp1->next=newNode;
            newNode->frequency=frequency;
            newNode->next=temp2;
            newNode->cTree=(struct cTree*)malloc(sizeof(struct cTree));
            (newNode->cTree)->data=(void *)data;
            (newNode->cTree)->binaryCode=NULL;
            (newNode->cTree)->leftNode=NULL;
            (newNode->cTree)->rightNode=NULL;
        }else{
            struct PQ *newNode=(struct PQ *)malloc(sizeof(struct PQ));
            newNode->frequency=frequency;
            newNode->next=*startPQ;
            newNode->cTree=(struct cTree*)malloc(sizeof(struct cTree));
            (newNode->cTree)->data=(void *)data;
            (newNode->cTree)->binaryCode=NULL;
            (newNode->cTree)->leftNode=NULL;
            (newNode->cTree)->rightNode=NULL;
            *startPQ=newNode;
        }
    }
    return *startPQ;
}//End of function insertInPQ

/*
    #This function will insert complete node in priority queue
    #This function will be called at the time of creating Huffman cTree
*/
void insertNodeInPQ(struct PQ *startPQ,struct PQ *newNode){
    struct PQ *temp=NULL;
    while(startPQ->frequency < newNode->frequency && startPQ->next){
        temp=startPQ;
        startPQ=startPQ->next;
    }
    if(!startPQ->next){
        startPQ->next=newNode;
        newNode->next=NULL;
    }else{
        temp->next=newNode;
        newNode->next=startPQ;
    }


}//End of function insertNodeInPQ

/*
    #This function will be called for deleting two first node from priority Queue PQ
*/
void deleteNodePQ(struct PQ **startPQ){
    struct PQ *temp=*startPQ;
    *startPQ=((*startPQ)->next)->next;
    free(temp->next);
    free(temp);
}//End of function deleteNodePQ

void traversePQ(struct PQ *startPQ){//function for traversing priority queue
    struct PQ *temp=startPQ;
    while(temp){
        //printf("%c %d\t",(temp->cTree)->data,temp->frequency);
        temp=temp->next;
    }
    //printf("\n");
}//End of traverse function for priority queue

/*
    #Most important function which assigns the codes
*/

void assignCodes(struct cTree *root,int *binaryCode,int len,int bit){
    if(root==NULL)return;
    else{
        int i=0;
        if(len==0){
            root->binaryCode=(int *)malloc(sizeof(int));
            root->binaryCode[0]=bit;
            len++;
        }else{
            len++;
            root->binaryCode=(int *)malloc(len*sizeof(int));
            while(i<len-1){
                root->binaryCode[i]=binaryCode[i];
                i++;
            }
            root->binaryCode[i]=bit;
        }
        int cl=(int)root->data;
        codeLength[cl]=len;
        codewords[cl]=(int)root->binaryCode;
        if(root->data!=(void*)EOF){
                //printf("%c  ",root->data);
                int k=0;
                while(k<=i){
                    //printf("%d",root->binaryCode[k]);
                    k++;
                }
                //printf("    %d\n",len);
        }
        assignCodes(root->leftNode,root->binaryCode,len,0);
        assignCodes(root->rightNode,root->binaryCode,len,1);

    }
}

void doCompression(void){
    char *fileName=(char*)malloc(100*sizeof(char));
    printf("Enter the name of file for compression:= ");
    scanf("%s",fileName);
    struct PQ *startPQ=NULL;//point to the address of priority queue First Node
    FILE *fo=fopen(fileName,"r");//opening file for compression
    if(fo==NULL){
        printf("\nFile Not Found");
        exit(0);
    }
    frequency=(int*)malloc(128*sizeof(int));//variable for getting in frequency
    codeLength=(int*)malloc(128*sizeof(int));
    codewords=(int*)malloc(128*sizeof(int));
    int i;
    for(i=0;i<128;i++){//initializing the frequency of each character
        frequency[i]=0;
        codeLength[i]=0;
        codewords[i]=0;
    }
    int countCharacters=0;
    char c;
    while((c=fgetc(fo))!=EOF){//counting the occurrences of characters
        countCharacters++;
        frequency[c]++;
    }

    i=0;
    while(i<128){
        if(frequency[i]>0){
            startPQ=insertInPQ(&startPQ,frequency[i],i);
        }
        i++;
    }
    traversePQ(startPQ);//traversing the priority queue generated
    //printf("\nConstructing Binary cTree\n");
    while(startPQ->next){//constructing Huffman cTree
        struct PQ *newNode=(struct PQ *)malloc(sizeof(struct PQ));
        newNode->frequency=startPQ->frequency+(startPQ->next)->frequency;
        newNode->next=NULL;
        newNode->cTree=(struct cTree*)malloc(sizeof(struct cTree));
        (newNode->cTree)->data=(void *)EOF;
        (newNode->cTree)->binaryCode=NULL;
        (newNode->cTree)->leftNode=startPQ->cTree;
        (newNode->cTree)->rightNode=(startPQ->next)->cTree;
        insertNodeInPQ(startPQ,newNode);
        deleteNodePQ(&startPQ);
        traversePQ(startPQ);
    }
    struct cTree *root=startPQ->cTree;
    assignCodes(root->leftNode,root->binaryCode,0,0);
    assignCodes(root->rightNode,root->binaryCode,0,1);

    /*
        Now calculating performance of algorithm
    */
    i=0;
    int cBits=0;
    int encodedCharacters=0;
    while(i<128){
        if(frequency[i]>0){
            cBits=cBits+(frequency[i]*codeLength[i]);
            encodedCharacters++;
        }
        i++;
    }
    rewind(fo);
    FILE *fw=fopen("compressed.huff","wb");
    int dai=0;
    int *dataArray=(int*)malloc(cBits*sizeof(int));
    while((c=fgetc(fo))!=EOF){//loop for reading the character and counting each characters
        int pos=(int)c;
        int i=0;
        int *j=(int*)codewords[pos];
        while(i<codeLength[pos]){
            dataArray[dai]=j[i];
            dai++;
            i++;
        }
    }

    /*
        #putting meta data for decompression into the file
    */
    fwrite(&encodedCharacters,sizeof(encodedCharacters),1,fw);
    fwrite(&cBits,sizeof(cBits),1,fw);
    for(i=0;i<128;i++){
        if(frequency[i]>0){
            int cLength=codeLength[i];
            char c=(char)i;
            fwrite(&c,sizeof(c),1,fw);
            fwrite(&cLength,sizeof(cLength),1,fw);
            //printf("%c  ",c);
            //printf("%d  ",cLength);
            int *characterCode=(int*)codewords[i];
            int j=0,codeRemaining=cLength;
            unsigned char data=0;
            while(codeRemaining-8>=0){
                codeRemaining-=8;
                data=0;
                int k=0;
                while(k<8){
                    if(characterCode[j]==0){
                        data=data<<1;
                        j++;
                        k++;
                    }else{
                        data=data<<1;
                        data |=(char)1;
                        k++;
                        j++;
                    }
                }
                //printf("%d  ",data);
                fwrite(&data,sizeof(data),1,fw);
            }
            if(codeRemaining!=0){
                int k=0;
                data=0;
                while(k<codeRemaining){
                    if(characterCode[j]==0){
                        data=data<<1;
                        j++;
                        k++;
                    }else{
                        data=data<<1;
                        data |=(char)1;
                        j++;
                        k++;
                    }
                }
                while(k<8){
                    data=data<<1;
                    k++;
                }
                fwrite(&data,sizeof(data),1,fw);
            }
        }
    }
    dai=0;//used for dataArray Index
    int countRemainingBits=0;
    unsigned char bitsHolder;
    while(dai<cBits){//loop for putting compressed data into file
        bitsHolder=0;
        countRemainingBits=cBits-dai;
        if(countRemainingBits-8>=0){
            int k=0;
            while(k<8){
                if(dataArray[dai]==0){
                    bitsHolder=bitsHolder<<1;
                    //printf("%d",dataArray[dai]);
                    k++;
                    dai++;
                }else{
                    bitsHolder=bitsHolder<<1;
                    bitsHolder |=1;
                    //printf("%d",dataArray[dai]);
                    k++;
                    dai++;
                }
            }
            fwrite(&bitsHolder,sizeof(bitsHolder),1,fw);
        }else{
            int k=0;
            while(k<countRemainingBits){
                if(dataArray[dai]==0){
                    bitsHolder=bitsHolder<<1;
                    //printf("%d",dataArray[dai]);
                    k++;
                    dai++;
                }else{
                    bitsHolder=bitsHolder<<1;
                    bitsHolder |=1;
                    //printf("%d",dataArray[dai]);
                    k++;
                    dai++;
                }
            }
            while(k<8){
                bitsHolder =bitsHolder<<1;
                k++;
            }
            fwrite(&bitsHolder,1,1,fw);
        }
    }

    /*
    #comparing the compressed and non compressed files
    */
    float uFileSize=countCharacters/1024;
    printf("\n\nSize of Uncompressed File is %.2f KB\n",uFileSize);
    float cFileSize=cBits/(1024*8);
    printf("Size of compressed File is %.2f KB\n\n\n\n",cFileSize);
    showOptions();
}

/*
#Starting code for Decompression
*/
struct dtree{//dtree structure for encoded characters
    char data;
    int codeLength;
    int *codeWord;
    struct dtree *left;
    struct dtree *right;
};
/*
    #This function is for creating dtree which have character and their codewords
*/
void insertIndtree(struct dtree **root,char data,int codeLength,int *codeWord){//
    if(*root==NULL){
        struct dtree *newNode=(struct dtree *)malloc(sizeof(struct dtree));
        newNode->data=data;
        newNode->codeLength=codeLength;
        newNode->codeWord=(int *)malloc(codeLength*sizeof(int));
        int i=0;
        while(i<codeLength){
            newNode->codeWord[i]=codeWord[i];
            i++;
        }
        newNode->left=NULL;
        newNode->right=NULL;
        *root=newNode;
    }else{
        if((*root)->codeLength >= codeLength){
            insertIndtree(&(*root)->left,data,codeLength,codeWord);
        }
        else{
            insertIndtree(&(*root)->right,data,codeLength,codeWord);
        }
    }
}//End of function Insert In dtree

void traversedtree(struct dtree *root){//this function traverse the dtree and print their values
    if(root==NULL)return;
    else{
        printf("%c  ",root->data);
        printf("%d  ",root->codeLength);
        int i=0;
        while(i<root->codeLength){
            //printf("%d",root->codeWord[i]);
            i++;
        }
        //printf("\n");
        traversedtree(root->left);
        traversedtree(root->right);
    }
}//end of traverse dtree function

int findNode(struct dtree *root,int codeLength,int *codeWord,FILE *fw){
    if(root==NULL){
        return 0;
    }
    else if(root->codeLength==codeLength){
        int i=0;
        while(i<codeLength){
            if(codeWord[i]!=root->codeWord[i]){
                break;
            }else
                i++;
        }
        if(i==codeLength){
            //printf("%c",root->data);
            fputc(root->data,fw);
            return 1;
        }else{
            findNode(root->left,codeLength,codeWord,fw);
        }
    }
    else if(root->codeLength>codeLength)
        findNode(root->left,codeLength,codeWord,fw);
    else
        findNode(root->right,codeLength,codeWord,fw);
}

void doDecompression(void){
        char *fileName=(char*)malloc(100*sizeof(char));
    printf("Enter the name of file for decompression:= ");
    scanf("%s",fileName);
    struct dtree *root=NULL;
    FILE *fo=fopen(fileName,"rb");//opening file for decompression
    if(fo==NULL){
        printf("\nFile Not Found");
        exit(0);
    }
    int encodedCharacters;//variable for holding total no of encoded characters
    int cBits;//variable for holding total no of bits of encoded data
    fread(&encodedCharacters,sizeof(encodedCharacters),1,fo);//reading byte for getting total no of encoded characters
    fread(&cBits,sizeof(cBits),1,fo);//reading 2byte for getting total no of bits of data
    //printf("%d ",encodedCharacters);
    //printf("%d ",cBits);
    int *dataArray=(int*)malloc(cBits*sizeof(int));//memory allocation for data Array;
    int i,loop=encodedCharacters;//variable for general index
    for(i=0;i<loop;i++){
        char data=0;
        fread(&data,sizeof(data),1,fo);
        int codeLength=0;
        fread(&codeLength,sizeof(codeLength),1,fo);
        int i=0,k=ceil((float)codeLength/8);
        unsigned char code;
        int *codeWord=(int*)malloc(codeLength*sizeof(int));
        int remainingBits=codeLength;
        int pos=0;
        while(i<k){
            fread(&code,sizeof(code),1,fo);
            int j=0;
            if(remainingBits-8>=0){
                remainingBits-=8;
                while(j<8){
                    char temp=code;
                    temp=temp<<j;
                    temp=temp>>7;
                    if(temp==-1)
                        temp=1;
                    codeWord[pos]=(int)temp;
                    j++;
                    pos++;
                }
            }else{
                j=0;
                while(j<remainingBits){
                    char temp=code;
                    temp=temp<<j;
                    temp=temp>>7;
                    if(temp==-1)
                        temp=1;
                    codeWord[pos]=(int)temp;
                    j++;
                    pos++;
                }
            }
            i++;
        }
        insertIndtree(&root,data,codeLength,codeWord);//inserting node in dtree
    }
    //traversedtree(root);
    unsigned char bitsHolder;
    int r=cBits%8;
    int Bits=cBits-r;
    //printf("%d  ",Bits);
    int dai=0;//Data Array index
    while(dai<Bits){
        fread(&bitsHolder,1,1,fo);
        int k=0;
        unsigned char data;
        while(k<8){
            if(dai>Bits)
                break;
            data=bitsHolder;
            data=data<<k;
            data=data>>7;
            dataArray[dai]=(int)data;
            k++;
            dai++;
        }
    }
    if(dai<cBits){
        fread(&bitsHolder,1,1,fo);
        int k=0;
        unsigned char data;
        while(k<r){
            data=bitsHolder;
            data=data<<k;
            data=data>>7;
            dataArray[dai]=(int)data;
            k++;
            dai++;
        }
    }

    //for(i=0;i<cBits;i++)
      //  printf("%d",dataArray[i]);

    fclose(fo);
    free(fo);
    /*
        #decompressing file
    */
    FILE *fw=fopen("ExpandMe.txt","w+");
    i=0;
    int decoded=0;
    //printf("\n");
    while(decoded<cBits){
        int *code=(int*)malloc((i+1)*sizeof(int));
        int k=0,dai=decoded;
        //printf("%d\n",decoded);
        while(k<=i){
            code[k]=dataArray[dai];
            k++;
            dai++;
        }
        if(findNode(root,i+1,code,fw)){
            i=0;
            decoded=dai;
        }else{
            i++;
        }
        free(code);
    }

    free(root);
    fclose(fw);
    free(fw);
    free(dataArray);
    printf("Decompressed Sucessfully\n\n\n\n");
    showOptions();
}

/*
#Finished code for Decompression
*/

/*
#Show Options
*/
void showOptions(){
    printf("\t\tText Compressor/De compressor\n\n\n");
    printf("Please Choose an Option\n");
    printf("1. Compress\n");
    printf("2. Decompress\n");
    printf("3. Exit\n\n\n");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1: doCompression();
            break;
        case 2: doDecompression();
            break;
        case 3: exit(0);
            break;
        default: showOptions();
    }
}
void main(void)
{
    showOptions();
}
