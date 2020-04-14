package ceng.ceng351.labdb;

import java.util.Vector;

public class Bucket {
    int localDepth;
    Vector elements;
    int bsize;

    public Bucket(int bs) {
        this.bsize = bs;
        elements = new Vector();
        this.localDepth = 1;
    }

    public Bucket(int bs, Vector rhs) {
        this.bsize = bs;
        elements = new Vector(rhs);
        this.localDepth = 1;
    }
    public boolean isFull() {
        return this.elements.size() == bsize;
    }
    public void addElement(String studentID) {
        this.elements.add(studentID);
    }

    public void deleteElement(String studentID) {
        elements.remove(studentID);
    }

    public void addBucket(Bucket b) {

    }
}
