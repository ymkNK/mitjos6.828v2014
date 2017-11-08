#include<stdio.h>
#include<pthread.h>
#define MAX 50
//最多需要生产的数量
pthread_mutex_t the_mutex;
pthread_cond_t condc,condp;
int buffer=0;
//生产者和消费者所使用的缓冲区
int count=0;
//用于记录消费的量，作为终止的条件
void *producer(void *ptr)
{
	while(count<=MAX)
	{
		pthread_mutex_lock(&the_mutex);
		//互斥使用缓冲区
		while(buffer>=10)pthread_cond_wait(&condp,&the_mutex);//如果缓冲区到达10个，那么生产者等待
		buffer++;
		printf("生产者生产了1件，当前缓冲区中有%d件\n",buffer );
		pthread_cond_signal(&condc);
		//唤醒消费者
		pthread_mutex_unlock(&the_mutex);
		//释放缓冲区
	}
	pthread_exit(0);
}

void *consumer(void *ptr)
{
	while(count<MAX)
	{
		pthread_mutex_lock(&the_mutex);
		//同上，互斥使用缓冲区
		while(buffer==0)pthread_cond_wait(&condc,&the_mutex);//如果缓冲区一个没有，那么消费者等待
		buffer--;
		count++;
		printf("消费者消费了1件，当前缓冲区中有%d件，消费历史总计%d件\n",buffer,count);
		//消费数量增加
		pthread_cond_signal(&condp);
		//唤醒生产者
		pthread_mutex_unlock(&the_mutex);
		//释放缓冲区
	}
	pthread_exit(0);
}

int main(int argc,char **argv)
{
	pthread_t pro1,pro2,pro3,con1,con2;//定义3个生产者，两个消费者
	pthread_mutex_init(&the_mutex,0);
	pthread_cond_init(&condc,0);
	pthread_cond_init(&condp,0);
	pthread_create(&con1,0,consumer,0);
	pthread_create(&con2,0,consumer,0);
	pthread_create(&pro1,0,producer,0);
	pthread_create(&pro2,0,producer,0);
	pthread_create(&pro3,0,producer,0);
	pthread_join(pro1,0);
	pthread_join(pro2,0);
	pthread_join(pro3,0);
	pthread_join(con1,0);
	pthread_join(con2,0);
}
