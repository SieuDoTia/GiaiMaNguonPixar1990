// Giải Mã Pixar
// Khởi động: 2021.08.08
// Phiên bản này: 2022.11.11

// Cho tìm và phân tích mã số cho phần mểm của Pixar từ đầu thập hiên 1990
// Gõ tên liên tiếp ví dụ: Showplace1 hay NetRenderman

#include <stdio.h>
#include <stdlib.h>

#define kSHOWPLACE_2    0x1e
#define kMAC_RENDERMAN  0x14
#define kTYPESTRY_2     0x11
#define kNET_RENDERMAN  0x10
#define kTYPESTRY_1     0x0f
#define kSHOWPLACE_1    0x0e   // ??? Dự đoán, chưa chứng kiến

#define kGIA_TRI_HOAC  0x11800


unsigned int maHoa( unsigned int giaTri ) {

   unsigned short chiSo = 0;
   
   while( chiSo < 0xed ) {  // 0xed == 237
//      printf( "%02x  %08x (%d)\n", chiSo, giaTri, giaTri );
      if( (giaTri & 0x01) != 0 ) {
         giaTri >>= 1;
         giaTri ^= 0x80000057;
         giaTri |= 0x80000000;
      }
      else {
         giaTri >>= 1;
      }
      
      chiSo++;
   }
   
   return giaTri;
}

// ---- Chỉ cho kiểm tra giá trị mã số
void kiemTraGiaTriMaSo( unsigned int giaTri ) {
   
   unsigned int giaTriMaHoa = maHoa( giaTri );
   
   // ---- chỉ giữ 17 bit thấp
   giaTriMaHoa &= 0x1ffff;

   // ---- kiểm tra có đầu == 0x11800
   unsigned int ketQuaHoac = giaTriMaHoa & 0x1ff00;

   if( ketQuaHoac != kGIA_TRI_HOAC ) {
      printf( "Giá trị %d không hợp lệ. Giải Mã Hóa: 0x%04x\n", giaTri, giaTriMaHoa );
      exit(0);
   }

   // ---- rút mã phần mềm
   unsigned char maPhanMem = giaTriMaHoa & 0xff;

   printf( "Kết qủa: 0x%04x  maPhanMem 0x%02x\n", giaTriMaHoa, maPhanMem );
   
   // ---- phát kết qủa
   if( maPhanMem == kSHOWPLACE_1 )
      printf( "--> Showplace 1??\n" );
   else if( maPhanMem == kTYPESTRY_1 )
      printf( "--> Typestry 1\n" );
   else if( maPhanMem == kNET_RENDERMAN )
      printf( "--> Net Renderman\n" );
   else if( maPhanMem == kTYPESTRY_2 )
      printf( "--> Typestry 2\n" );
   else if( maPhanMem == kMAC_RENDERMAN )
      printf( "--> MacRenderman\n" );
   else if( maPhanMem == kSHOWPLACE_2 )
      printf( "--> Showplace 2\n" );
   else
      printf( "--> Không biết luôn!\n" );
}


unsigned int phanTichTenPhanMemHoacMaSo( char *tenPhanMem ) {
   
   // ---- lấy ký từ đầu cho xem
   char kyTuDau = tenPhanMem[0];
   int maDangTim;
   
   // ==== phân tích mã số
   // ---- chỉ cho kiểm tra giá trị mã số 0 --> 9
   if( (kyTuDau > 0x2f) && (kyTuDau < 0x3a ) ) {
      unsigned int giaTriMaSo;

      sscanf( tenPhanMem, "%u", &giaTriMaSo );

      kiemTraGiaTriMaSo( giaTriMaSo );

      exit(0);
   }

   // ==== phân tích tên phần mềm
   else if( kyTuDau == 's' || kyTuDau == 'S' ) {
      // ---- đến ký từ cuối
      char *kyTu = tenPhanMem++;
      while( *tenPhanMem != 0x00 ) {
         tenPhanMem++;
      }
      
      // ---- trở lại một ký từ
      tenPhanMem--;
      
      if( *tenPhanMem == '1' ) {
         maDangTim = kSHOWPLACE_1;
         printf( " Showplace 1\n" );
      }
      
      else { //( kyTuDau == '2' )
         maDangTim = kSHOWPLACE_2;
         printf( " Showplace 2\n" );
      }
   }
   
   else if( kyTuDau == 'm' || kyTuDau == 'M' ) {
      maDangTim = kMAC_RENDERMAN;
      printf( " MacRenderman\n" );
   }
   
   else if( kyTuDau == 't' || kyTuDau == 'T' ) {
      // ---- đến ký từ cuối
      char *kyTu = tenPhanMem++;
      while( *tenPhanMem != 0x00 ) {
         tenPhanMem++;
      }
      
      // ---- trở lại một ký từ
      tenPhanMem--;
      
      if( *tenPhanMem == '1' ) {
         maDangTim = kTYPESTRY_1;
         printf( " Typestry 1\n" );
      }
      
      else { //( kyTuDau == '2' )
         maDangTim = kTYPESTRY_2;
         printf( " Typestry 2\n" );
      }
   }
   
   else { // if( kyTuDau == 'n' || kyTuDau == 'N' )
      maDangTim = kNET_RENDERMAN;
      printf( " Net Renderman\n" );
   }
   
   return maDangTim;
}


#pragma mark ==== MAIN.c
int main( int argc, char *argv[] ) {
   
   // ---- MẶC ĐỊNH: mã đang tìm (tùy phần mềm)
   unsigned  int maDangTim = 0;
   
   if( argc > 1 ) {
      // ---- phân tích tên phần mềm hoặc mã số
      maDangTim = phanTichTenPhanMemHoacMaSo( argv[1] );
   }
   else {
      maDangTim = kNET_RENDERMAN;
      printf( " Net Rendeman\n" );  // MẶC ĐỊNH
   }

   // ---- 'HOẶC' số 0x11800
   maDangTim |= kGIA_TRI_HOAC;
   
   // ---- giá trị cho bắt đầu tìm
   unsigned int giaTri = 111;
   unsigned int soLuongHopLe = 1;  // đếm số lượng hợp lệ
   
   while( giaTri < 0xffffffff ) {
   unsigned int giaTriMaHoa = maHoa( giaTri );

      // ---- chỉ giữ 17 bit thấp
      giaTriMaHoa &= 0x1ffff;

      // ---- so sánh giaTri và 0x11800 |  
      if( giaTriMaHoa == maDangTim ) {
         printf( " %d:  %u\n", soLuongHopLe, giaTri );
         soLuongHopLe++;
      }
      
      giaTri++;
   }
   
   // ---- cho biết số lượng hợp lệ
   printf( "Số lượng hợp lệ: %d\n", soLuongHopLe );
   
   return 0;
}

