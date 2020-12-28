package project.utility;

import project.parts.Arm;
import project.parts.Base;
import project.parts.Part;
import project.parts.logics.Builder;
import project.parts.logics.Fixer;
import project.parts.logics.Inspector;
import project.parts.logics.Supplier;
import project.parts.payloads.Camera;
import project.parts.payloads.Gripper;
import project.parts.payloads.MaintenanceKit;
import project.parts.payloads.Welder;

import java.beans.IntrospectionException;
import java.beans.PropertyDescriptor;
import java.lang.reflect.*;
import java.util.List;
import java.util.Random;


public class Common
{
    public static Random random = new Random() ;

    public static synchronized Object get (Object object , String fieldName )
    {
        // TODO
        // This function retrieves (gets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: get!" )



        // I got the idea of needed methods from:
        // https://stackoverflow.com/questions/16171637/java-reflection-how-to-get-field-value-from-an-object-not-knowing-its-class
        try {
            Class currentObjectClass = object.getClass(); // get object class
            Field requiredField = currentObjectClass.getDeclaredField(fieldName); // get its field
            requiredField.setAccessible(true);
            Object val = requiredField.get(object);
            requiredField.setAccessible(false);
            return val;
        }
        catch (Exception e) {
            throw new SmartFactoryException("Failed: get!");
        }
    }

    public static synchronized void set ( Object object , String fieldName , Object value )
    {
        // TODO
        // This function modifies (sets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: set!" )


        // inspired from getter
        try {
            Class currObjClass = object.getClass();
            Field reqField = currObjClass.getDeclaredField(fieldName);
            reqField.setAccessible(true);
            reqField.set(object, value);
            reqField.setAccessible(false);
        } catch (Exception e) {
            throw new SmartFactoryException("Failed: set!");
        }

    }

    public static synchronized String getClassName(String str) {
        // getClass().toString() returns something like "Class project.parts.logic.Builder" so this function takes this
        // form as input, and returns "Builder" part as output
        String[] tokens = str.split("\\.");
        str = tokens[tokens.length - 1];
        return str;
    }



    // unused
    public static synchronized boolean isInList(List<Part> list, Part part) {
        Object partNo = Common.get(part, "serialNo");
        for(Part l : list) {
            Object lNo = Common.get(l, "serialNo");
            if(partNo.equals(lNo)) return true;
        }
        return false;
    }


    // for Factory class
    public static synchronized Base produceBase(int sNo) {
        return new Base(sNo);
    }

    // for Factory class
    public static synchronized Part producePart(String name) {
        switch (name) {
            //case "arm":
            case "Arm":
                return new Arm();
            //case "gripper":
            case "Gripper":
                return new Gripper();
            //case "supplier":
            case "Supplier":
                return new Supplier();
            //case "welder":
            case "Welder":
                return new Welder();
            //case "builder":
            case "Builder":
                return new Builder();
            //case "camera":
            case "Camera":
                return new Camera();
            //case "inspector":
            case "Inspector":
                return new Inspector();
            //case "maintenanceKit":
            case "MaintenanceKit":
                return new MaintenanceKit();
            //case "fixer":
            case "Fixer":
                return new Fixer();
            default:
                throw new SmartFactoryException("Failed: createPart!");
        }
    }
}