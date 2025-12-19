#include <stdio.h>
enum type{
num_rbs,numerology,numerology_ssb,dl_timimg_params,ul_timimg_params,dl_iq_comp,ul_iq_comp,ssb_iq_comp,prach_iq_comp,dl_sections_per_sym,ul_sections_per_sym,
ssb_sections_per_sym,frames_per_sym,frames_per_sym_ssb,dest_mac_addr,src_mac_addr,eAxC_ID,ru_ports
};
struct str1{
int ccid;
int num_rbs;

}s1;
struct str2{

   int ccid;
   int num_rbs;
   int extended_cp;
}s2;
struct str3{
int ccid;
int numerology;
int extended_cp;

}s3;


struct str4{
 int ccid;
 int delay_comp_cp;
 int delay_comp_up;
 int advance;


}s4;


struct str5{
int ccid;
int delay_comp_cp;
int advance;
int radio_ch_delay;
}s5;
struct str6{
int ccid;
int bit_width;
int comp_method;
int mplane
}s6;
struct str7{
int ccid;
int bit_width;
int comp_method;
int mplane

}s7;

struct str8{
int ccid;
int bit_width;
int comp_method;
int mplane

}s8;
struct str9{
int ccid;
int bit_width;
int comp_method;
int mplane

}s9;
struct str10{
    int ccid;
    int num_sect;
    int num_ctrl;
}s10;
struct str11{
    int ccid;
    int num_sect;
    int num_ctrl;
}s11;
struct str12{
    int ccid;
    int num_sect;
    int num_ctrl;

}s12;
struct str13{
int ccid;
int num_frames;
}s13;
struct str14{
int ccid;
int num_frames;
}s14;
struct str15{
int ccid;
char d_mac_addr[100];
}s15;
struct str16{
int ccid;
char s_mac_addr[100];
}s16;
struct str17{
int ccid;
int DU_bits;
int BS_bits;
int CC_bits;
int RU_bits;
}s17;
struct str18{
int ccid;
int RU_bits;
int ss_bits;
int mask;
int user_value;
int prach_value;
int ssb_value;

}s18;
void update();
void set_config(int type,void*ptr);

int main() {



  set_config(num_rbs,&s1);
  set_config(numerology,&s2);
  set_config(numerology_ssb,&s3);
  set_config(dl_timimg_params,&s4);
  set_config(ul_timimg_params,&s5);
  set_config(dl_iq_comp,&s6);
  set_config(ul_iq_comp,&s7);
  set_config(ssb_iq_comp,&s8);
  set_config(prach_iq_comp,&s9);
  set_config(dl_sections_per_sym,&s10);
  set_config(ul_sections_per_sym,&s11);
  set_config(ssb_sections_per_sym,&s12);
  set_config(frames_per_sym,&s13);
  set_config(frames_per_sym_ssb,&s14);
  set_config(dest_mac_addr,&s15);
  set_config(src_mac_addr,&s16);
  set_config(eAxC_ID,&s17);
  set_config(ru_ports,&s18);

update();
   return 0;
}

void set_config(int type,void *ptr){

    switch(type)
{
case num_rbs:
    printf("Enter ccid: ");
   scanf("%d", &s1.ccid);
   printf("Enter num_rbs:");
   scanf("%d", &s1.num_rbs);
    printf("\n");
    break;
 case numerology:

    printf("Enter ccid: ");
   scanf("%d", &s2.ccid);
   printf("Enter num_rbs: ");
   scanf("%d", &s2.num_rbs);
    printf("Enter extended_cp: ");
   scanf("%d", &s2.extended_cp);
   printf("\n");
   break;
  case numerology_ssb:
    printf("Enter ccid: ");
   scanf("%d", &s3.ccid);
   printf("Enter numerology: ");
   scanf("%d", &s3.numerology);
    printf("Enter extended_cp: ");
   scanf("%d", &s3.extended_cp);
   printf("\n");
    break;
  case dl_timimg_params:
       printf("Enter ccid: ");
   scanf("%d", &s4.ccid);
   printf("Enter delay_comp_cp: ");
   scanf("%d", &s4.delay_comp_cp);
    printf("Enter delay_comp_up: ");
   scanf("%d", &s4.delay_comp_up);
    printf("Enter advance: ");
   scanf("%d", &s4.advance);
   printf("\n");
   break;
  case ul_timimg_params:
       printf("Enter ccid: ");
   scanf("%d", &s5.ccid);
   printf("Enter :delay_comp_cp:");
   scanf("%d", &s5.delay_comp_cp);
    printf("Enter :advance ");
   scanf("%d", &s5.advance);
     printf("Enter :radio_ch_delay:");
   scanf("%d", &s5.radio_ch_delay);
   printf("\n");
   break;
  case dl_iq_comp:
       printf("Enter ccid: ");
   scanf("%d", &s6.ccid);
   printf("Enter bit_width: ");
   scanf("%d", &s6.bit_width);
    printf("Enter comp_method: ");
   scanf("%d", &s6.comp_method);
     printf("Enter mplane: ");
   scanf("%d", &s6.mplane);
   printf("\n");

    break;
  case ul_iq_comp:
       printf("Enter ccid: ");
   scanf("%d", &s7.ccid);
   printf("Enter bit_width: ");
   scanf("%d", &s7.bit_width);
    printf("Enter comp_method: ");
   scanf("%d", &s7.comp_method);
     printf("Enter mplane: ");
   scanf("%d", &s7.mplane);
   printf("\n");
   break;
case ssb_iq_comp:
     printf("Enter ccid: ");
   scanf("%d", &s8.ccid);
  printf("Enter bit_width: ");
   scanf("%d", &s8.bit_width);
    printf("Enter comp_method: ");
   scanf("%d", &s8.comp_method);
     printf("Enter mplane: ");
   scanf("%d", &s8.mplane);
   printf("\n");
    break;
case prach_iq_comp:
     printf("Enter ccid: ");
   scanf("%d", &s9.ccid);
  printf("Enter bit_width: ");
   scanf("%d", &s9.bit_width);
    printf("Enter comp_method: ");
   scanf("%d", &s9.comp_method);
     printf("Enter mplane: ");
   scanf("%d", &s9.mplane);
   printf("\n");
   break;
case dl_sections_per_sym:
     printf("Enter ccid: ");
   scanf("%d", &s10.ccid);
    printf("Enter num_sect: ");
   scanf("%d", &s10.num_sect);
    printf("Enter num_ctrl: ");
   scanf("%d", &s10.num_ctrl);
    printf("\n");
   break;

case ul_sections_per_sym:
     printf("Enter ccid: ");
   scanf("%d", &s11.ccid);
    printf("Enter num_sect: ");
   scanf("%d", &s11.num_sect);
    printf("Enter num_ctrl: ");
   scanf("%d", &s11.num_ctrl);
    printf("\n");
   break;
case ssb_sections_per_sym:
     printf("Enter ccid: ");
   scanf("%d", &s12.ccid);
    printf("Enter num_sect: ");
   scanf("%d", &s12.num_sect);
    printf("Enter num_ctrl: ");
   scanf("%d", &s12.num_ctrl);
    printf("\n");
   break;
case frames_per_sym:
     printf("Enter ccid: ");
   scanf("%d", &s13.ccid);
    printf("Enter num_frames: ");
   scanf("%d", &s13.num_frames);
    printf("\n");
    break;
case frames_per_sym_ssb:
     printf("Enter ccid: ");
   scanf("%d", &s14.ccid);
    printf("Enter num_frames: ");
   scanf("%d", &s14.num_frames);
    printf("\n");
    break;
case dest_mac_addr:
     printf("Enter ccid: ");
   scanf("%s", &s15.ccid);
    printf("Enter d_mac_addr: ");
   scanf("%s",&s15.d_mac_addr);
    printf("\n");
    break;

case src_mac_addr:
     printf("Enter ccid: ");
   scanf("%d", &s16.ccid);
    printf("Enter s_mac_addr: ");
   scanf("%s", &s16.s_mac_addr);
    printf("\n");
    break;

case eAxC_ID:
    printf("Enter ccid: ");
   scanf("%d", &s17.ccid);
   printf("Enter DU_bits: ");
   scanf("%d", &s17.DU_bits);
   printf("Enter BS_bits: ");
   scanf("%d", &s17.BS_bits);
   printf("Enter CC_bits: ");
   scanf("%d", &s17.CC_bits);
   printf("Enter RU_bits: ");
   scanf("%d", &s17.RU_bits);
    printf("\n");
   break;
case ru_ports:

    printf("Enter ccid: ");
   scanf("%d", &s18.ccid);
   printf("Enter RU_bits: ");
   scanf("%d", &s18.RU_bits);
   printf("Enter ss_bits: ");
   scanf("%d", &s18.ss_bits);
   printf("Enter mask: ");
   scanf("%d", &s18.mask);
   printf("Enter user_value: ");
   scanf("%d", &s18.user_value);
   printf("Enter prach_value: ");
   scanf("%d", &s18.prach_value);
   printf("Enter ssb_value: ");
   scanf("%d", &s18.ssb_value);
   break;


}

}

void update(){

FILE * fp ;
fp = fopen ( "Guru.txt", " w " );
  if(fp==NULL)
  {
          printf("file not open");
  }
  else
  {
      printf("file open\n");
         fprintf(fp,"# Frame format");
         fprintf(fp,"\n");
        fprintf(fp,"set num_rbs %d %d\n",s1.ccid,s1.num_rbs);
        fprintf(fp,"set numerology %d %d %d\n",s2.ccid,s2.num_rbs,s2.extended_cp);
        fprintf(fp,"set numerology_ssb %d %d %d\n",s3.ccid,s3.numerology,s3.extended_cp);
         fprintf(fp,"\n");
        fprintf(fp,"# Define DL and UL time advanced");
         fprintf(fp,"\n");
        fprintf(fp,"set dl_timing_params %d %d %d %d\n",s4.ccid,s4.delay_comp_cp,s4.delay_comp_up,s4.advance);
        fprintf(fp,"set ul_timing_params %d %d %d %d\n",s5.ccid,s5.delay_comp_cp,s5.advance,s5.radio_ch_delay);
         fprintf(fp,"\n");
        fprintf(fp,"# IQ compression parameters");
         fprintf(fp,"\n");
        fprintf(fp,"set dl_iq_comp %d %d %d %d\n",s6.ccid,s6.bit_width,s6.comp_method,s6.mplane);
        fprintf(fp,"set ul_iq_comp %d %d %d %d\n",s7.ccid,s7.bit_width,s7.comp_method,s7.mplane);
        fprintf(fp,"set ssb_iq_comp %d %d %d %d\n",s8.ccid,s8.bit_width,s8.comp_method,s8.mplane);
        fprintf(fp,"set prach_iq_comp %d %d %d %d\n",s9.ccid,s9.bit_width,s9.comp_method,s9.mplane);
         fprintf(fp,"\n");
        fprintf(fp,"# Section and frames per symb limits");
         fprintf(fp,"\n");

        fprintf(fp,"dl_sections_per_sym %d %d %d\n",s10.ccid,s10.num_sect,s10.num_ctrl);
        fprintf(fp,"set ul_sections_per_sym %d %d %d\n",s11.ccid,s11.num_sect,s11.num_ctrl);
        fprintf(fp,"set ssb_sections_per_sym %d %d %d\n",s12.ccid,s11.num_sect,s12.num_ctrl);
        fprintf(fp,"set frames_per_sym %d %d\n",s13.ccid,s13.num_frames);
        fprintf(fp,"set frames_per_sym_ssb %d %d\n",s14.ccid,s14.num_frames);
         fprintf(fp,"\n");
        fprintf(fp,"# set L2-L4 parameters");
        fprintf(fp,"\n");

         fprintf(fp,"set dest_mac_addr %d %s",s15.ccid,s15.d_mac_addr);
         fprintf(fp,"\n");
        fprintf(fp,"set src_mac_addr %d %s",s16.ccid,s16.s_mac_addr);
         fprintf(fp,"\n");
         fprintf(fp,"# set RTCIP/PCID parameters");
         fprintf(fp,"\n");
        fprintf(fp,"set eAxC_ID %d %d %d %d %d",s17.ccid,s17.DU_bits,s17.BS_bits,s17.CC_bits,s17.RU_bits);
        fprintf(fp,"\n");
        fprintf(fp,"set ru_ports %d %d %d %d %d %d %d",s18.ccid,s18.RU_bits,s18.ss_bits,s18.mask,s18.user_value,s18.prach_value,s18.ssb_value);



   fclose ( fp ) ;
  }

}






