import java.io.*;
import java.util.*;

public class ScriptOutputProcessor {

    public static void main(String args[]) throws IOException {
	Scanner s = new Scanner(System.in);
	double[][][] simple = new double[4][4][40]; // i is datatype, j is datasize, k is num_threads
	for (int i = 0, j = 0; i < 40 && s.hasNextLine();) {
	    String line = s.nextLine();
	    if (line.startsWith("Benchmark")) continue;
	    if (j == 0) {
		simple[0][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[0][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[0][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[0][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    } else if (j == 1) {
		simple[1][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[1][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[1][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[1][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    } else if (j == 2) {
		simple[2][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[2][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[2][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[2][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    }
	    else if (j == 3) {
		simple[3][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[3][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[3][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simple[3][3][i] = Double.parseDouble(line.split(",")[1]);
		j = 0; ++i;
	    }
	}

	double[][][]  simd = new double[4][4][40];
	for (int i = 0, j = 0; i < 40 && s.hasNextLine();) {
	    String line = s.nextLine();
	    if (line.startsWith("Benchmark")) continue;
	    if (j == 0) {
		simd[0][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[0][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[0][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[0][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    } else if (j == 1) {
		simd[1][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[1][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[1][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[1][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    } else if (j == 2) {
		simd[2][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[2][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[2][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[2][3][i] = Double.parseDouble(line.split(",")[1]);
		++j;
	    } else if (j == 3) {
		simd[3][0][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[3][1][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[3][2][i] = Double.parseDouble(line.split(",")[1]);
		line = s.nextLine();
		simd[3][3][i] = Double.parseDouble(line.split(",")[1]);
		j = 0; ++i;
	    }
	}

	for (int j = 0; j < 4; ++j) { // datasize
	    for (int k = 0; k < 40; ++k) { // threads
		for (int i = 0; i < 4; ++i) { // datatype
		    System.out.print(simple[i][j][k]+","+simd[i][j][k]+",");
		}
		System.out.println();
	    }
	    System.out.println("===");
	}
    }

}
