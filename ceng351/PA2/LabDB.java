package ceng.ceng351.labdb;


import java.lang.Math;
import java.util.Collection;
import java.util.Vector;



public class LabDB {
    int globalDepth;
    Vector<Bucket> arr = new Vector(2);
    int bs;


    public LabDB(int bucketSize) {
        this.bs = bucketSize;
        this.globalDepth = 1;
        double num = Math.pow(2,globalDepth);
        for(int i=0; i<num; i++) {
            Bucket b = new Bucket(bucketSize);
            arr.add(b);
        }
    }

    public String decodeToBinaryString(int id, int length) {
        StringBuilder result = new StringBuilder(Integer.toBinaryString(id));
        int len = result.length();
        if(length == len) return result.toString();
        else if(length < len) return result.substring(len-length);
        else {
            while(len != length) {
                result.insert(0, "0");
                len++;
            }
            return result.toString();
        }

    }

    public int indexCalculator(String studentID, int length) {
        String[] str = studentID.split("e");
        int num = Integer.parseInt(str[1]);
        String eval = decodeToBinaryString(num, length);
        int index = Integer.parseInt(eval,2);
        return index;
    }

    public void enter(String studentID) {
        if(search(studentID) != "-1") return;
        int index = indexCalculator(studentID, globalDepth);
        Bucket curr_bucket = arr.get(index);
        int size = (int) Math.pow(2, globalDepth);

        if(globalDepth == curr_bucket.localDepth) {
            if(!curr_bucket.isFull()) this.arr.get(index).addElement(studentID);
            else extendHash(index, size, studentID);
        }

        else {
            int start_index = indexCalculator(studentID, curr_bucket.localDepth);
            if(!arr.get(start_index).isFull()) arr.get(start_index).addElement(studentID);
            else extendHash(start_index, size, studentID);
        }

    }


    public void extendHash(int extendIndex, int size, String studentID) {
            arr.get(extendIndex).localDepth++;
            Bucket b = new Bucket(bs);
            b.localDepth = arr.get(extendIndex).localDepth;
            Vector<String> temp = new Vector<String>(arr.elementAt(extendIndex).elements);
            temp.add(studentID);

            if(arr.get(extendIndex).localDepth == globalDepth+1) {
                globalDepth++;
                for(int i=0; i<size; i++) {
                    Bucket bucket = arr.get(i);
                    arr.add(bucket);
                }
                arr.get(extendIndex).elements.clear();
                arr.set(extendIndex, b);
            }

            else {
                arr.set(size/2+extendIndex,b);
                arr.get(extendIndex).elements.clear();
            }

            for(int j=0; j<temp.size(); j++) enter(temp.get(j).toString());
            temp.clear();
    }


    public void leave(String studentID) {
        if(search(studentID) == "-1") return;
        int index = indexCalculator(studentID, globalDepth);
        int factor = (int) Math.pow(2,arr.get(index).localDepth-1);
        int size = (int) Math.pow(2, globalDepth);
        Bucket curr_bucket = arr.get(index);
        curr_bucket.deleteElement(studentID);
        if(factor+index < size && curr_bucket.elements.isEmpty() && curr_bucket.localDepth == arr.get(factor+index).localDepth) mergeBuckets(index,factor);
    }

    public void mergeBuckets(int index, int factor) {
        if(globalDepth == 1) return;
        int size = arr.size();
        Bucket curr = arr.get(index);
        arr.get(index + factor).localDepth--;
        for(int i=0; i<size;i++) {
            if(arr.get(i) == curr) {
                arr.set(i, arr.get(index + factor));
                arr.get(i).localDepth = arr.get(index + factor).localDepth;
            }
        }

        while(!checker()) compactTable();

    }

    public boolean checker() {
        for(int i=0; i<arr.size(); i++) {
            if(arr.get(i).localDepth == globalDepth) {
                return true;
            }
        }
        return false;
    }

    public void compactTable() {
        globalDepth--;
        int size = (int) Math.pow(2, globalDepth);
        for(int i=0; i<size; i++) {
            arr.remove(size);
        }
    }

    public String search(String studentID) {
        int index = indexCalculator(studentID, globalDepth);
        if(arr.get(index).localDepth == globalDepth) {
            if(arr.get(index).elements.contains(studentID)) {
                String[] str = studentID.split("e");
                int num = Integer.parseInt(str[1]);
                String eval = decodeToBinaryString(num, globalDepth);
                return eval;
            }

            else return "-1";
        }
        else {
            int size = (int) Math.pow(2,globalDepth);
            int ld = arr.get(index).localDepth;
            int factor = (int) Math.pow(2,ld);
            int start = indexCalculator(studentID, ld);
            for(int i=start; i<size; i+=factor) {
                if(arr.get(i).elements.contains(studentID)) {
                    String[] str = studentID.split("e");
                    int num = Integer.parseInt(str[1]);
                    String eval = decodeToBinaryString(num, globalDepth);
                    return eval;
                }
            }
            return "-1";

        }
    }


    public void printLab() {
        System.out.println("Global depth : "+ globalDepth);
        int num = (int) Math.pow(2, globalDepth);
        for(int i=0; i<num; i++) {
            System.out.print(decodeToBinaryString(i, globalDepth)  + " : [Local depth:" + arr.elementAt(i).localDepth + "]");
            int size = arr.elementAt(i).elements.size();
            Bucket elm = arr.elementAt(i);
            for (int j=0; j<size; j++) {
                System.out.print("<" + elm.elements.get(j) + ">");
            }
            System.out.println();
        }
    }

}
