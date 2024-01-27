#include "AlienData.h"

AlienTypes operator|(const AlienTypes& lhs, const AlienTypes& rhs) {
	return static_cast<AlienTypes>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

AlienTypes operator&(const AlienTypes& lhs, const AlienTypes& rhs) {
	return static_cast<AlienTypes>(static_cast<int>(lhs) & static_cast<int>(rhs));
}