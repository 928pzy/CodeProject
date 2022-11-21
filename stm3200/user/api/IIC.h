#ifndef _IIC_H_
#define _IIC_H_

void IIC_Init (void);
void IIC_WritePage(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth);
void IIC_Roundread(u8 Addr,u8 Subaddr,u8 *Data,u16 Lenth);
void IIC_Test (void);
void IIC_Display(u8 *buffer);
void Clear_buffer (u8 *Buffer,u16 Lenth);


void IIC_Sive (void);





#endif 

