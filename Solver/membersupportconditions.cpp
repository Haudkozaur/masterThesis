#include "MemberSupportConditions.h"

namespace SolverFEM {

// Constructor to initialize support conditions for the member
MemberSupportConditions::MemberSupportConditions(int memberId, int startNodeId, int endNodeId,
                                                 bool startTx, bool startTz, bool startRy,
                                                 bool endTx, bool endTz, bool endRy)
    : memberId(memberId), startNodeId(startNodeId), endNodeId(endNodeId),
    startTx(startTx), startTz(startTz), startRy(startRy),
    endTx(endTx), endTz(endTz), endRy(endRy) {}

// Getters for member support conditions
int MemberSupportConditions::getMemberId() const {
    return memberId;
}

int MemberSupportConditions::getStartNodeId() const {
    return startNodeId;
}

int MemberSupportConditions::getEndNodeId() const {
    return endNodeId;
}

bool MemberSupportConditions::getStartTx() const {
    return startTx;
}

bool MemberSupportConditions::getStartTz() const {
    return startTz;
}

bool MemberSupportConditions::getStartRy() const {
    return startRy;
}

bool MemberSupportConditions::getEndTx() const {
    return endTx;
}

bool MemberSupportConditions::getEndTz() const {
    return endTz;
}

bool MemberSupportConditions::getEndRy() const {
    return endRy;
}

} // namespace SolverFEM
