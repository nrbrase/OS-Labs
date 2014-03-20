#include "userprog/syscall.h"
//1518886046250407618
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);
static void syscall_write (struct intr_frame *);
void syscall_read (struct intr_frame *);
int syscall_open (struct intr_frame *);
void syscall_create (struct intr_frame *);
void syscall_tell (struct intr_frame *);
void syscall_seek (struct intr_frame *);
void syscall_remove (struct intr_frame *);
void syscall_close (struct intr_frame *);
void syscall_exit (int status);
void syscall_filesize (struct intr_frame *);

static void syscall_exec (struct intr_frame *);
bool copy_data(uint8_t *udst, uint8_t *src, int size);
bool check_pointer(uint8_t *udst, int size);
struct lock file_lock;
struct fd* get_fd(int fd);
void close_fd(struct thread* th);


void
syscall_init (void) 
{
  
  
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_lock);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	int *sp = f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
//	printf("%d syscall\n",*sp );
	switch(*sp)	{	
		case SYS_EXIT:
		  thread_current()->exit_status=0;
		  int status=0;
		  if(is_kernel_vaddr (sp+1))
		  	syscall_exit(-1);
		  status = *(sp+1);
		  syscall_exit(status);
			break;
		case SYS_HALT:
			//printf ("2write call!\n");
			power_off();
			break;
		case SYS_EXEC:
			//printf ("3write call!\n");
		       syscall_exec(f);
			break;
		case SYS_WAIT:
			;
			int pid=0;
			if(is_kernel_vaddr (sp+1))
		  	syscall_exit(-1);
		         pid = *(sp+1);
			f->eax = process_wait(pid);
			//printf ("4write call!\n");
			break;
		case SYS_CREATE:
		        syscall_create(f);
			break;
		case SYS_REMOVE:
		  syscall_remove(f);
			//printf ("6write call!\n");
			break;
		case SYS_OPEN:
		        syscall_open(f);
			break;
		case SYS_FILESIZE:
		//	printf ("8write call!\n");
		  syscall_filesize(f);
			break;
		case SYS_READ:
		  syscall_read(f);
		//	printf ("9write call!\n");
			break;
		case SYS_WRITE:
			syscall_write(f);
			// printf ("1111write call!\n"); 
			break;
		case SYS_SEEK:
		  syscall_seek(f);
			//printf ("10write call!\n");
			break;
		case SYS_TELL:
		  syscall_tell(f);
			//printf ("11write call!\n");
			break;
		case SYS_CLOSE:
		  syscall_close(f);
			//printf ("12write call!\n");
			break;
		default:
			//printf ("13system call!\n");
			thread_current()->exit_status =-1;
			thread_exit ();
	}
}
static void
syscall_write(struct intr_frame *f){
	int *sp = f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
	int fd= *(sp+1);
	char *buffer = (char *)(*(sp+2));
	int size = *(sp+3);
	//printf("hmmmm.\n");
	if(!check_pointer((uint8_t *)buffer,size)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
	struct thread *t = thread_current();
	//TODO check user's buffer. can't trust it.

	if(fd==1){
		f->eax = size;
		while(size>100){
			putbuf(buffer,100);
			buffer=buffer+100;
			size=size-100;

		}
		putbuf(buffer,size);
	}
	else if(get_fd(fd) != NULL){
	//	printf("dnjd\n");
	  f->eax = file_write(get_fd(fd)->file,buffer, size);
	}
	else{
		f->eax =0;
	}
}



void
syscall_read(struct intr_frame *f){
        int *sp = f->esp;
		if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
	int fd= *(sp+1);
	// printf("here\n");
	char *buffer = (char *)(*(sp+2));
	int size = *(sp+3);
	if(!check_pointer((uint8_t *)buffer,size)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
	struct thread *t = thread_current();
	int i=0;
	if(fd==0){
		f->eax = size;
		while(size>0){
			buffer[i]=input_getc();
			//buffer=buffer+100;
			size=size-1;
			i++;
		}
		/* getch(buffer,size); */
	}
	else if(get_fd(fd) != NULL){
	  // printf("here\n");
	  f->eax = file_read((get_fd(fd))->file,buffer, size);
	}
	else{
		f->eax =0;
	}
}

void syscall_remove(struct intr_frame *f){  
  int *sp= f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
  char *filename=(char *)(*(sp+1));
  if(!is_user_vaddr(filename)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
  if(!filesys_remove(filename))f->eax=-1;
     else f->eax=1;
     
}
 void
syscall_tell(struct intr_frame *f){
        int *sp = f->esp;
       if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
	int fd= *(sp+1);
	 struct thread *th= thread_current();
	 file_tell(get_fd(fd)->file);
 }

void syscall_create(struct intr_frame *f){  
  int *sp= f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
  char *filename=(char *)(*(sp+1));
  if(!is_user_vaddr(filename)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
 
  int size=*(sp+2);
  if(!is_user_vaddr(sp+2)||!is_user_vaddr(sp+1)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
  if(filename==NULL || size <0){
    // printf("incorrect arguments");
    syscall_exit(-1);
  }
/* printf( " %s \n",filename); */
 /* printf("wwwww\n"); */
  /* lock_acquire(&file_lock); */
 /* printf("wwww1w\n"); */
  
  
  
  if(!filesys_create(filename,size)){
    f->eax = 0;
    /* printf("Are we here.\n"); */
    /* lock_release(&file_lock); */
    /* printf("Are we here.\n"); */
  }
  else {
  	f->eax=1; 
/* printf("Are we here.\n"); */	
    /* lock_release(&file_lock); */
  }
  /* printf("wwwww\n"); */
    /* printf("create succesful\n"); */
}

static void
syscall_exec(struct intr_frame *f){
  int *sp=f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
  char *filename=(char *)(*(sp+1));
  if(!is_user_vaddr(filename)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
 	//printf("%s\n",filename );
  f->eax=process_execute(filename);
  
}
void syscall_seek(struct intr_frame *f){  
  int *sp= f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1); 
 int fd=*(sp+1);
  int position=*(sp+2);
  struct thread *th= thread_current();
  file_seek(get_fd(fd)->file,position);
    }
	  
void syscall_close(struct intr_frame *f){  
  int *sp= f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);  
  int fd=*(sp+1);
  //struct thread *th= thread_current();
  struct fd *des;
 // printf("hmm\n");
  des = get_fd(fd);
//  printf("dfb h%s\n",des );
//  if(des==NULL) syscall_exit(-1);
  lock_acquire(&file_lock);
  file_close(des->file);
  lock_release(&file_lock);
  f->eax=1;
  list_remove(&des->file_elem);
  //close_fd(th);
}

void syscall_filesize(struct intr_frame *f){  
  int *sp= f->esp;
 // printf("syscall_filesize\n");
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
	//printf("eeee\n");
  int fd=*(sp+1);
  //f->eax=0;
  struct thread *th= thread_current();
  //if(th->files[fd]==NULL) syscall_exit(-1);
  f->eax= file_length(get_fd(fd)->file);
 // printf("%d syscall_filesize\n",f->eax );
}

int syscall_open(struct intr_frame *f){
  struct fd* descriptor;
   int *sp=f->esp;
	if(is_kernel_vaddr (f->esp))
	  syscall_exit(-1);
   int index=-1;
   char *filename=(char *)(*(sp+1));
   struct thread *th= thread_current();
   if(!is_user_vaddr(filename)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
 if(!is_user_vaddr(sp+1)) {
	  //printf("invalid buffer");
	  syscall_exit(-1);
	}
   if(filename==NULL)syscall_exit(-1);
   descriptor= malloc(sizeof *descriptor);
   /* lock_acquire(&file_lock); */
   struct file *file1 = filesys_open(filename);
   /* lock_release(&file_lock); */
   //th->des->file=file1;
   descriptor->file=file1;
  //  printf("do we here1 \n");     
   if(file1==NULL){
     f->eax=-1;
     // lock_release(&file_lock);
     free(descriptor);
     return -1;
   }

   index= descriptor->num=th->next_fd++;
   f->eax=index;
 //  printf("ebnhehb\n");
   list_push_front(&th->fdl,&descriptor->file_elem);
  // printf("ebnhehb\n");

/*  struct list_elem *e;
  struct fd* des=NULL;
 // struct thread* th= thread_current();
    for (e = list_begin (&th->fdl); e != list_end (&th->fdl);
	     e = list_next (e)){
      des=list_entry(e , struct fd, file_elem);
      //if(des->num==fd)return des;
      //else des=NULL;
    	printf("iter %d\n",des->num);
    }*/
      
 //   printf("do we here3 %d \n",index);
   /* printf("Index: %d \n",index);   */
   return index;
   
}



void
syscall_exit(int status){
  //int *sp= f->esp;
	 struct thread *th= thread_current();
	 th->exit_status=status;
	 th->c->exit_status=status;
	// printf("chiilll %d\n",status);
	 close_fd(th);
	/* printf("%s: exit(%d)\n", thread_name(),f->error_code); */
	 printf("%s: exit(%d)\n", th->name,th->exit_status);
	 //destroy childs files and resources here;
	 thread_exit();

}


/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */

 bool
check_pointer(uint8_t *userpointer,int size){
  if(((int)userpointer)<PHYS_BASE &&
     ((int)userpointer+size)<PHYS_BASE)
  return true;
  else return false;
}

static int
get_user (const uint8_t *uaddr)
{
  ASSERT((int)uaddr<PHYS_BASE);
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}
 
/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  
  ASSERT((int)udst<PHYS_BASE);
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

bool copy_data(uint8_t *udst, uint8_t *src, int size)
{
  int i =0;
  for(;i<size;i++){
    if(put_user((char *)(udst+i),(char *)(src+i))==false)
      return false;
  }
  return true;
}

void close_fd(struct thread* th){
  struct list_elem *e,*e2;
  struct fd* des=NULL;
  
  if(th==NULL) th=thread_current();

  e = list_begin (&th->fdl);
  lock_acquire(&file_lock);
  while(e!=list_end (&th->fdl)){
  	e2 = list_next (e);
  	des= list_entry(e , struct fd, file_elem);
  	file_close(des->file);
    list_remove(&(des->file_elem));
    free(des);
    e=e2;
  }
  lock_release(&file_lock);

/*  for (e = list_begin (&th->fdl); e != list_end (&th->fdl);
	     e = list_next (e)){
    des= list_entry(e , struct fd, file_elem);
  	printf("ww1.5 %d\n",des->num);
    file_close(des->file);
    list_remove(&(des->file_elem));
    free(des);
  }*/

}
    
struct fd* get_fd(int fd){
  struct list_elem *e;
  struct fd* des=NULL;
  struct thread* th= thread_current();
    for (e = list_begin (&th->fdl); e != list_end (&th->fdl);
	     e = list_next (e)){
      des=list_entry(e , struct fd, file_elem);
      if(des->num==fd) {	
      		return des;
      }
     // else des=NULL;
    }
  syscall_exit(-1);
    //return des;
}
