package project.parts.logics;

import project.SimulationRunner;
import project.components.Robot;
import project.parts.Base;
import project.parts.Part;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.ArrayList;
import java.util.List;

public class Builder extends Logic {
    @Override
    public void run(Robot robot) {
        // TODO

        // The builder checks all parts on the production line to see if there
        // is a possibility to fulfill one production step. A production step might be
        // to take an arm and attach it to a base, or to take a payload and attach it
        // to a base-arm, or to finish a robot by adding the correct logic chip, or to
        // pick up a completed robot and add it alongside of worker robots (to speed
        // up the production), or to store the completed robot if there is enough
        // storage space. If there is no possibility of fulfilling a production step, then
        // the builder waits. The builder also signals the factory to stop production
        // once the storage space is full.

        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Builder cannot build anything, waiting!%n", ...);
        // System.out.printf("Robot %02d : Builder woke up, going back to work.%n", ...);
        // System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", ...);


        try {


            Object serialNo = Common.get(robot, "serialNo");
            System.out.printf("Robot %02d : Builder woke up, going back to work.%n", serialNo);


            boolean isFixed = false;
            List<Part> baseParts = new ArrayList<>();

            synchronized (SimulationRunner.factory.productionLine.parts) {
                List<Part> parts = SimulationRunner.factory.productionLine.parts;
                for (Part part : parts) {
                    if (Common.getClassName(part.getClass().toString()).equals("Base")) { // add bases to a list && loop in it later
                        baseParts.add(part);
                    }
                }

                for (Part basePart : baseParts) {

                    Object arm = Common.get(basePart, "arm"); // for convenience
                    Object payload = Common.get(basePart, "payload");
                    Object logic = Common.get(basePart, "logic");

                    if (arm == null && payload == null && logic == null) { // basePart is only a base, attach an arm to it
                        for (Part part : parts) {
                            if (Common.getClassName(part.getClass().toString()).equals("Arm")) { // if part is an arm
                                Common.set(basePart, "arm", part); // set this arm as the arm of base
                                isFixed = true;
                                parts.remove(part); // and remove it from the list
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            }
                        }

                    } else if (payload == null && logic == null) { // basePart needs both payload && logic to be completed
                        for (Part part : parts) {
                            String currPartClassName = Common.getClassName(part.getClass().toString());
                            // if part is any kind of payload
                            if (currPartClassName.equals("Welder") || currPartClassName.equals("MaintenanceKit") || currPartClassName.equals("Gripper") || currPartClassName.equals("Camera")) { // if currPart is a payload
                                Common.set(basePart, "payload", part); // set the part as base-arm's payload
                                isFixed = true;
                                parts.remove(part); // remove it from list
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            }
                        }

                    } else if (logic == null) { // all basePart needs to be a robot is a chip, find suitable one
                        String payloadName = Common.getClassName(payload.getClass().toString());
                        for (Part part : parts) {
                            String currPartName = Common.getClassName(part.getClass().toString());
                            // try to find suitable logic for given payload
                            if (payloadName.equals("Welder") && currPartName.equals("Builder")) {
                                Common.set(basePart, "logic", part);
                                isFixed = true;
                                parts.remove(part);
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            } else if (payloadName.equals("MaintenanceKit") && currPartName.equals("Fixer")) {
                                Common.set(basePart, "logic", part);
                                isFixed = true;
                                parts.remove(part);
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            } else if (payloadName.equals("Gripper") && currPartName.equals("Supplier")) {
                                Common.set(basePart, "logic", part);
                                isFixed = true;
                                parts.remove(part);
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            } else if (payloadName.equals("Camera") && currPartName.equals("Inspector")) {
                                Common.set(basePart, "logic", part);
                                isFixed = true;
                                parts.remove(part);
                                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
                                synchronized (SimulationRunner.productionLineDisplay) {
                                    SimulationRunner.productionLineDisplay.repaint();
                                }
                                break;
                            }
                        }


                    } else { // basePart is a completed robot
                        if (SimulationRunner.factory.robots.size() < SimulationRunner.factory.maxRobots) { // there's room for new completed robot in working robots
                            synchronized (SimulationRunner.factory.robots) { // add completed robot to robots
                                SimulationRunner.factory.robots.add((Robot) basePart);
                                new Thread((Robot) basePart).start(); // make new robot work
                            }

                            synchronized (SimulationRunner.robotsDisplay) {
                                SimulationRunner.robotsDisplay.repaint();
                            }
                            isFixed = true;

                            SimulationRunner.factory.productionLine.parts.remove(basePart); // remove it from productionLine
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                        } else if (SimulationRunner.factory.storage.robots.size() < SimulationRunner.factory.storage.maxCapacity) { // there's room for new completed robot in storage
                            synchronized (SimulationRunner.factory.storage.robots) { // add completed robot to storage
                                SimulationRunner.factory.storage.robots.add((Robot) basePart);
                            }
                            synchronized (SimulationRunner.storageDisplay) {
                                SimulationRunner.storageDisplay.repaint();
                            }
                            SimulationRunner.factory.productionLine.parts.remove(basePart); // remove from productionLine
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            isFixed = true;
                        }
                        if (SimulationRunner.factory.storage.robots.size() == SimulationRunner.factory.storage.maxCapacity) { // if storage is full
                            SimulationRunner.factory.initiateStop(); // stop robots
                            synchronized (SimulationRunner.factory.robots) {
                                SimulationRunner.factory.robots.notifyAll(); // and notify them
                            }
                        }
                        if (isFixed) break; // so that one builder will build only one base and stop
                    }
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

//        List<Part> toRemove = new ArrayList<Part>();
//        int indexToDelete = -1;
//        Part basePart = null;
//        // int partSize = parts.size();
//
//        boolean isChanged = false;
//
//        for(Part part : parts) {
//            String partClass = Common.getClassName(part.getClass().toString());
//
//            //synchronized (part) {
//                if(partClass.equals("Base")) { // if the part is base;
//                    Object arm = Common.get(part, "arm");
//                    Object payload = Common.get(part, "payload");
//                    Object logic = Common.get(part, "logic");
//
//                    if(arm == null && payload == null && logic == null) { // if current part is just a single base, traverse all parts, find an arm && attach it to this base
//                        for(int j=0; j<parts.size(); j++) {
//                            Part currPart = parts.get(j);
//
//                            String currPartClassName = Common.getClassName(currPart.getClass().toString());
//
//                            if(currPartClassName.equals("Arm")) {
//                                Common.set(part, "arm", currPart);
//                                indexToDelete = j;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                isChanged = true;
//                                // SimulationRunner.productionLineDisplay.repaint();
//                                break;
//                            }
//                        }
//                    }
//
//                    else if(payload == null && logic == null) { // current part is a base-arm
//                        for(int j=0; j<parts.size(); j++) {
//                            Part currPart = parts.get(j);
//                            String currPartClassName = Common.getClassName(currPart.getClass().toString());
//                            if(currPartClassName.equals("Welder") || currPartClassName.equals("MaintenanceKit") || currPartClassName.equals("Gripper") || currPartClassName.equals("Camera")) { // if currPart is a payload
//                                Common.set(part, "payload", currPart);
//                                indexToDelete = j;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                isChanged = true;
//                                // SimulationRunner.productionLineDisplay.repaint();
//                                break;
//                            }
//                        }
//                    }
//
//                    else if(logic == null) { // current part only needs a chip
//                        String payloadName = Common.getClassName(payload.getClass().toString());
//                        for(int j=0; j<parts.size(); j++) {
//                            Part currPart = parts.get(j);
//                            String currPartName = Common.getClassName(currPart.getClass().toString());
//                            if(payloadName.equals("Welder") && currPartName.equals("Builder")) {
//                                Common.set(part, "logic", currPart);
//                                isChanged = true;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                indexToDelete = j;
//                                break;
//                            }
//                            else if(payloadName.equals("MaintenanceKit") && currPartName.equals("Fixer")) {
//                                Common.set(part, "logic", currPart);
//                                isChanged = true;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                indexToDelete = j;
//                                break;
//                            }
//                            else if(payloadName.equals("Gripper") && currPartName.equals("Supplier")) {
//                                Common.set(part, "logic", currPart);
//                                isChanged = true;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                indexToDelete = j;
//                                break;
//                            }
//                            else if(payloadName.equals("Camera") && currPartName.equals("Inspector")) {
//                                Common.set(part, "logic", currPart);
//                                isChanged = true;
//                                /*synchronized (parts) {
//                                    parts.remove(j);
//                                    // partSize--;
//                                }*/
//                                indexToDelete = j;
//                                break;
//                            }
//                            /*if(isChanged) {
//                                SimulationRunner.productionLineDisplay.repaint();
//                            }*/
//                        }
//                    }
//
//
//                    else { // current part is a completed one
//                        if(SimulationRunner.factory.robots.size() < SimulationRunner.factory.maxRobots) { // there's room for new completed robot in robots
//                            synchronized (SimulationRunner.factory.robots) { // add completed robot to robots
//                                SimulationRunner.factory.robots.add((Robot) part);
//                            }
//                            SimulationRunner.robotsDisplay.repaint();
//                            synchronized (SimulationRunner.factory.productionLine.parts) { // and delete from production line
//                                SimulationRunner.factory.productionLine.parts.remove(part);
//                            }
//                            SimulationRunner.productionLineDisplay.repaint();
//                            isChanged = true;
//
//                        }
//
//                        else if(SimulationRunner.factory.storage.robots.size() < SimulationRunner.factory.storage.maxCapacity) { // there's room for new completed robot in storage
//                            synchronized (SimulationRunner.factory.storage.robots) { // add completed robot to storage
//                                SimulationRunner.factory.storage.robots.add((Robot) part);
//                            }
//                            SimulationRunner.storageDisplay.repaint();
//                            synchronized (SimulationRunner.factory.productionLine.parts) { // and delete from production line
//                                SimulationRunner.factory.productionLine.parts.remove(part);
//                            }
//                            SimulationRunner.productionLineDisplay.repaint();
//                            isChanged = true;
//
//                        }
//
//                        else { // storage is also full, stop production !!
//                            SimulationRunner.factory.initiateStop();
//                        }
//                    }
//
//                    if(indexToDelete != -1) { // some changes has been made
//                        synchronized (parts) {
//                            parts.remove(indexToDelete);
//                        }
//                        SimulationRunner.productionLineDisplay.repaint();
//                        System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", serialNo);
//                    }
//                }
//            //}
//
//
//
//        }


