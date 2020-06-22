#ifndef DS_EXCEPTIONS_H
#define DS_EXCEPTIONS_H

class UnderflowException { };
class IllegalArgumentException { };
class ArrayIndexOutOfBoundsException { };
class IteratorOutOfBoundsException { };
class IteratorMismatchException { };
class IteratorUninitializedException { };

//RequestExceptions are used for input validation into the AVL tree.
struct RequestException {
public:
	std::string content;
};
//This exception is to be thrown then the UID is unable to be found in the tree.
class UIDNotFoundException : public RequestException {
public:
	UIDNotFoundException() {
		content = "The specified UID was not found.";
	}
};
//This exception is to be thrown when a user attempts to access a message with an incorrect password.
class InvalidPasswordException : public RequestException {
public:
	InvalidPasswordException(std::string remainingAttempts) {
		content = "Invalid password. You have " + remainingAttempts + " attempt(s) remaining before the message self-destructs.";
	}
};
//This exception is to be thrown when a user runs out of tries when accessing a message.
class MessageDestructedException : public RequestException {
public:
	MessageDestructedException() {
		content = "The message you attempted to retrieve has self destructed.";
	}
};

//This exception is used to block the use of functions that either no longer work for our tree
//or are not usable due to security reasons.
class MethodDepreciatedException { };

#endif