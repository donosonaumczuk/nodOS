#include <semaphoreTest.h>

int semaphoreShow(int argumentQuantity, void **argumentVector) {
	uint64_t pid[3];
	char *relatorSem = "relator";
	char *kungFurySem = "kungFury";
	char *hackerManSem = "hackerMan";
	void **arguments;
	createSemaphore(relatorSem, 0);
	createSemaphore(kungFurySem, 0);
	createSemaphore(hackerManSem, 0);
	*arguments = relatorSem;
	*(arguments + 1) = kungFurySem;
	*(arguments + 2) = hackerManSem;
	pid[0] = createProcess(&relator, 3, arguments);
	pid[1] = createProcess(&kungFury, 3, arguments);
	pid[2] = createProcess(&hackerMan, 3, arguments);
	waitChild(pid[0]);
	waitChild(pid[1]);
	waitChild(pid[2]);
	return 0;
}
// Hacker Man: I was able to triangulate the cell phone signal and trace the caller. His name is Adolf Hitler.
// Kung Fury: Hitler. He's the worst criminal of all time.

// Hacker Man: Do you know him, sir?
// Kung Fury: I guess you could say that. In the 1940s, Hitler was a kung-fu champion. He was so good at kung-fu, that he decided to change his name to 'Kung Führer'. But he didn't stop there. He knew the Kung Fury prophecy, and wanted to claim the throne, so he and his posse of Nazi soldiers conducted experiments for years. They were never able to figure how to master the art of Kung Fury. Then one day, he disappeared from the face of the Earth. And no one has seen him ever since, until now. I guess if he figured he can find the Chosen One and kill him, He will become Kung Fury himself. He must have traveled in time in order to get here, which is probably why he disappeared in the first place.

// Hacker Man: So what are you gonna do?
// Kung Fury: My job. I'm gonna go back in time to Nazi Germany and kill Hitler once and for all.

// Hacker Man: So, uh, how are you gonna do that?
// Kung Fury: I'm not sure. I need some sort of time machine.

// [Hackerman thinks]
// Hacker Man: Wait a minute. Using an RX modulator, I might be able to conduct a mainframe cell direct and hack the uplink to the download.
// Kung Fury: What the hell does that mean?

// Hacker Man: It means that with the right computer algorithms, I can hack you back in time. Just like a time machine.
// Kung Fury: Well then. It's hacking time.
int relator(int argumentQuantity, void ** argumentVector) {
	char *relatorSem = (char *)(argumentVector[0]);
	char *kungFurySem = (char *)(argumentVector[1]);
	char *hackerManSem = (char *)(argumentVector[2]);
	semaphoreWait(relatorSem);
	printf("relator: [Hackerman thinks]\n");
	semaphorePost(hackerManSem);
	return 0;
}

int kungFury(int argumentQuantity, void ** argumentVector) {
	char *relatorSem = (char *)(argumentVector[0]);
	char *kungFurySem = (char *)(argumentVector[1]);
	char *hackerManSem = (char *)(argumentVector[2]);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: Hitler. He's the worst criminal of all time.\n");
	semaphorePost(hackerManSem);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: I guess you could say that. In the 1940s, Hitler was a kung-fu champion. He was so good at kung-fu, that he decided to change his name to 'Kung Führer'. But he didn't stop there. He knew the Kung Fury prophecy, and wanted to claim the throne, so he and his posse of Nazi soldiers conducted experiments for years. They were never able to figure how to master the art of Kung Fury. Then one day, he disappeared from the face of the Earth. And no one has seen him ever since, until now. I guess if he figured he can find the Chosen One and kill him, He will become Kung Fury himself. He must have traveled in time in order to get here, which is probably why he disappeared in the first place.\n");
	semaphorePost(hackerManSem);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: My job. I'm gonna go back in time to Nazi Germany and kill Hitler once and for all.\n");
	semaphorePost(hackerManSem);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: I'm not sure. I need some sort of time machine.\n");
	semaphorePost(relatorSem);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: What the hell does that mean?\n");
	semaphorePost(hackerManSem);
	semaphoreWait(kungFurySem);
	printf("Kung Fury: Well then. It's hacking time.\n");
	return 0;
}

int hackerMan(int argumentQuantity, void ** argumentVector) {
	char *relatorSem = (char *)(argumentVector[0]);
	char *kungFurySem = (char *)(argumentVector[1]);
	char *hackerManSem = (char *)(argumentVector[2]);
	printf("Hacker Man: I was able to triangulate the cell phone signal and trace the caller. His name is Adolf Hitler.\n");
	semaphorePost(kungFurySem);
	semaphoreWait(hackerManSem);
	printf("Hacker Man: Do you know him, sir?\n");
	semaphorePost(kungFurySem);
	semaphoreWait(hackerManSem);
	printf("Hacker Man: So what are you gonna do?\n");
	semaphorePost(kungFurySem);
	semaphoreWait(hackerManSem);
	printf("Hacker Man: So, uh, how are you gonna do that?\n");
	semaphorePost(kungFurySem);
	semaphoreWait(hackerManSem);
	printf("Hacker Man: Wait a minute. Using an RX modulator, I might be able to conduct a mainframe cell direct and hack the uplink to the download.\n");
	semaphorePost(kungFurySem);
	semaphoreWait(hackerManSem);
	printf("Hacker Man: It means that with the right computer algorithms, I can hack you back in time. Just like a time machine.\n");
	semaphorePost(kungFurySem);
	return 0;		

}