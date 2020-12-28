package project.parts.logics;

import project.SimulationRunner;
import project.components.Factory;
import project.components.Robot;
import project.displays.RobotsDisplay;
import project.utility.Common;

public class Fixer extends Logic {
    @Override
    public void run(Robot robot) {
        // TODO

        // The fixer fetches the first broken robot from the broken robots
        // list. Then, it fixes and wakes up the broken robot. If there are no broken
        // robots in the list, then the fixer waits.

        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n", ...);
        // System.out.printf("Robot %02d : Nothing to fix, waiting!%n", ...);
        // System.out.printf("Robot %02d : Fixer woke up, going back to work.%n", ...);


        Object serialNo = Common.get(robot, "serialNo");
        try {
            System.out.printf("Robot %02d : Fixer woke up, going back to work.%n", serialNo);
            boolean fixed = false;


            synchronized (SimulationRunner.factory.brokenRobots) {
                if (SimulationRunner.factory.brokenRobots.size() == 0) { // no broken robots
                    System.out.printf("Robot %02d : Nothing to fix, waiting!%n", serialNo);
                    SimulationRunner.factory.brokenRobots.wait();
                } else { // brokenRobots isnt empty
                    if (SimulationRunner.factory.brokenRobots.size() != 0) {
                        Robot toBeFixedRobot = SimulationRunner.factory.brokenRobots.get(0); // pick first robot to fix
                        Object toBeFixedRobotSerialNo = Common.get(toBeFixedRobot, "serialNo"); // for printf
                        Object arm = Common.get(toBeFixedRobot, "arm");
                        Object logic = Common.get(toBeFixedRobot, "logic");
                        Object payload = Common.get(toBeFixedRobot, "payload");

                        if (arm == null) { // if toBeFixedRobot has no arm
                            Common.set(toBeFixedRobot, "arm", Factory.createPart("Arm"));
                            fixed = true;
                        }

                        if (logic == null) { // if toBeFixedRobot has no logic
                            if (payload == null) { // and also has no payload, you can generate logic randomly
                                int randomLogic = Common.random.nextInt(4);
                                switch (randomLogic) {
                                    case 0:
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Welder"));
                                        break;
                                    case 1:
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Fixer"));
                                        break;
                                    case 2:
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Inspector"));
                                        break;
                                    case 3:
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Supplier"));
                                        break;
                                }
                            } else { // generate suitable logic for given payload
                                String payloadType = Common.getClassName(payload.getClass().toString());
                                switch (payloadType) {
                                    case "Welder":
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Builder"));
                                        break;
                                    case "MaintenanceKit":
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Fixer"));
                                        break;
                                    case "Camera":
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Inspector"));
                                        break;
                                    case "Gripper":
                                        Common.set(toBeFixedRobot, "logic", Factory.createPart("Supplier"));
                                        break;
                                }
                            }
                            fixed = true;
                        }

                        if (payload == null) { // when we are here, logic cannot be null (would be handled in previous if stmt)
                            String logicType = Common.getClassName(logic.getClass().toString());
                            switch (logicType) {
                                case "Builder":
                                    Common.set(toBeFixedRobot, "payload", Factory.createPart("Welder"));
                                    break;
                                case "Fixer":
                                    Common.set(toBeFixedRobot, "payload", Factory.createPart("MaintenanceKit"));
                                    break;
                                case "Inspector":
                                    Common.set(toBeFixedRobot, "payload", Factory.createPart("Camera"));
                                    break;
                                case "Supplier":
                                    Common.set(toBeFixedRobot, "payload", Factory.createPart("Gripper"));
                                    break;
                            }
                            fixed = true;
                        }

                        if(fixed) { // if toBeFixedRobot is modified (precaution)
                            synchronized (toBeFixedRobot) {
                                toBeFixedRobot.notify();
                            }
                            System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n", serialNo, toBeFixedRobotSerialNo);
                        }

                        synchronized (SimulationRunner.robotsDisplay) {
                            SimulationRunner.robotsDisplay.repaint();
                        }
                        if (SimulationRunner.factory.brokenRobots.size() != 0 && SimulationRunner.factory.brokenRobots.get(0) == toBeFixedRobot) { // just for precaution
                            SimulationRunner.factory.brokenRobots.remove(0); // remove toBeFixedRobot from brokenRobots so you wont try to fix it again
                        }
                    }
                }
            }

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}