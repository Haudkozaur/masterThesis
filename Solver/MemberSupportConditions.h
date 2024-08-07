#ifndef MEMBERSUPPORTCONDITIONS_H
#define MEMBERSUPPORTCONDITIONS_H

namespace SolverFEM {

class MemberSupportConditions
{
private:
    int memberId; // Unique ID for the member
    int startNodeId; // Node ID at the start of the member
    int endNodeId; // Node ID at the end of the member
    bool startTx; // Constraint on displacement in the x-direction at start node
    bool startTz; // Constraint on displacement in the z-direction at start node
    bool startRy; // Constraint on rotation about the y-axis at start node
    bool endTx; // Constraint on displacement in the x-direction at end node
    bool endTz; // Constraint on displacement in the z-direction at end node
    bool endRy; // Constraint on rotation about the y-axis at end node

public:
    // Constructor to initialize support conditions for the member
    MemberSupportConditions(int memberId, int startNodeId, int endNodeId,
                            bool startTx, bool startTz, bool startRy,
                            bool endTx, bool endTz, bool endRy);

    // Getters for member support conditions
    int getMemberId() const;
    int getStartNodeId() const;
    int getEndNodeId() const;
    bool getStartTx() const;
    bool getStartTz() const;
    bool getStartRy() const;
    bool getEndTx() const;
    bool getEndTz() const;
    bool getEndRy() const;
};

} // namespace SolverFEM

#endif // MEMBERSUPPORTCONDITIONS_H
