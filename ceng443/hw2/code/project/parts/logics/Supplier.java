package project.parts.logics;

import project.SimulationRunner;
import project.components.Factory;
import project.components.Robot;
import project.parts.Part;
import project.utility.Common;

public class Supplier extends Logic {
    @Override
    public void run(Robot robot) {
        // TODO

        // If the production line is not full, the supplier randomly gen-
        // erates a robot part and places it at the next available location on the
        // production line. If the production line is full, the supplier removes a ran-
        // domly selected part on the production line. After completing either action,
        // the supplier notifies the builders.

        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Supplying a random part on production line.%n", ...);
        // System.out.printf( "Robot %02d : Production line is full, removing a random part from production line.%n", ...);
        // System.out.printf( "Robot %02d : Waking up waiting builders.%n", ...);


        try {

            // First, determine what to generate (if you will). Then, if there's enough room in productionline,
            // generate it. else, delete a random part
            // sync before "if" cases so guarantee that parts wont be modified after entering to the if case

            Object serialNo = Common.get(robot, "serialNo"); // to use with printf
            int rand = Common.random.nextInt(4); // random number to generate base/arm/payloaad/logic
            int maxProduction = SimulationRunner.factory.productionLine.maxCapacity;
            switch (rand) {
                case 0: // Base
                    synchronized (SimulationRunner.factory.productionLine.parts) {
                        if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add base
                            System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                            SimulationRunner.factory.productionLine.parts.add(Factory.createBase());
                        } else { // line is full, delete random
                            System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                            SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                        }
                    }
                    synchronized (SimulationRunner.productionLineDisplay) {
                        SimulationRunner.productionLineDisplay.repaint();
                    }
                    System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                    /*synchronized (SimulationRunner.factory.robots) {
                        for(Robot r : SimulationRunner.factory.robots) {
                            Object builderLogic = Common.get(r, "logic");
                            if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                r.notify(); // notify builders
                            }
                        }
                    }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                    break;

                case 1: // Arm
                    synchronized (SimulationRunner.factory.productionLine.parts) {
                        if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add arm
                            System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                            SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Arm"));
                        } else { // line is full, delete random
                            System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                            SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));

                        }
                    }
                    synchronized (SimulationRunner.productionLineDisplay) {
                        SimulationRunner.productionLineDisplay.repaint();
                    }
                    System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                    /*synchronized (SimulationRunner.factory.robots) {
                        for(Robot r : SimulationRunner.factory.robots) {
                            Object builderLogic = Common.get(r, "logic");
                            if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                r.notify(); // notify builders
                            }
                        }
                    }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                    break;

                case 2: // Logic
                    int randomLogic = Common.random.nextInt(4);
                    switch (randomLogic) {
                        case 0: // Builder
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add builder
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Builder"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;

                        case 1: // Fixer
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add fixer
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Fixer"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;

                        case 2: // Inspector
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add inspector
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Inspector"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;

                        case 3: // Supplier
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add supplier
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Supplier"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;
                    }
                    break;

                case 3: // Payload
                    int randomPayload = Common.random.nextInt(4);
                    switch (randomPayload) {
                        case 0: // Camera
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add camera
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Camera"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;

                        case 1: // Gripper
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add gripper
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Gripper"));

                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;


                        case 2: // MaintenanceKit
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add maintenancekit
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("MaintenanceKit"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;


                        case 3: // Welder
                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                if (SimulationRunner.factory.productionLine.parts.size() < maxProduction) { // add welder
                                    System.out.printf("Robot %02d : Supplying a random part on production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.add(Factory.createPart("Welder"));
                                } else { // line is full, delete random
                                    System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", serialNo);
                                    SimulationRunner.factory.productionLine.parts.remove(Common.random.nextInt(maxProduction));
                                }
                            }
                            synchronized (SimulationRunner.productionLineDisplay) {
                                SimulationRunner.productionLineDisplay.repaint();
                            }
                            System.out.printf("Robot %02d : Waking up waiting builders.%n", serialNo);
                            /*synchronized (SimulationRunner.factory.robots) {
                                for(Robot r : SimulationRunner.factory.robots) {
                                    Object builderLogic = Common.get(r, "logic");
                                    if(builderLogic != null && Common.getClassName(builderLogic.toString()).equals("Builder")) {
                                        r.notify(); // notify builders
                                    }
                                }
                            }*/

                            synchronized (SimulationRunner.factory.productionLine.parts) {
                                SimulationRunner.factory.productionLine.parts.notifyAll();
                            }
                            break;
                    }
                    break;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}


