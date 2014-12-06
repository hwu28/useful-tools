import java.awt.*;
import java.awt.image.*;
import java.util.*;
import java.io.*;
import javax.imageio.*;

public class GaussianBlur
{
	public static double[] getKernel1D(double sigma)
	{
		int l = (int)Math.ceil(3*sigma);
		int w = l*2+1;
		double []kernel = new double[w];
		double sum = 0;
		for (int i = -l; i <= l; i++)
			sum += kernel[i+l] = Math.exp(-i*i/(2.0*sigma*sigma))/Math.sqrt(2.0*Math.PI*sigma*sigma);
		//normalize the kernel
		for (int i = -l; i <= l; i++)
			kernel[i+l] /= sum;
		return kernel;
	}
	public static BufferedImage blur(BufferedImage img, double []kernel)
	{
		int l = kernel.length/2, ntrun = 0;
		BufferedImage ret = new BufferedImage(img.getWidth(), img.getHeight(), BufferedImage.TYPE_INT_ARGB);
		double [][][][]fin = new double[2][4][img.getWidth()][img.getHeight()];

		for (int w = 0; w < img.getWidth(); w++)
			for (int h = 0; h < img.getHeight(); h++)
				for (int i = 0; i < 4; i++)
					fin[0][i][w][h] = ((img.getRGB(w, h) & (0xFF << i*8)) >>> i*8);
		for (int d = 0; d < 2; d++)
		{
			for (int w = 0; w < img.getWidth(); w++)
				for (int h = 0; h < img.getHeight(); h++)
					for (int i = 0; i < 4; i++)
						fin[(d+1)%2][i][w][h] = 0;
			for (int w = 0; w < img.getWidth(); w++)
				for (int h = 0; h < img.getHeight(); h++)
					for (int a = -l; a <= l; a++)
					{
						int nw = w, nh = h;
						if (d == 0)
							nw += a;
						else
							nh += a;
						if (nw < 0 || nw >= img.getWidth() || nh < 0 || nh >= img.getHeight())
							continue;
						for (int i = 0; i < 4; i++)
							fin[(d+1)%2][i][nw][nh] += fin[d][i][w][h] * kernel[a+l];
					}
		}
		for (int w = 0; w < img.getWidth(); w++)
			for (int h = 0; h < img.getHeight(); h++)
			{
				int px = 0, val;
				for (int i = 0; i < 4; i++)
				{
					val = (int)Math.round(fin[0][i][w][h]);
					if (val > 255)
					{
						ntrun++;
						val = 255;
					}
					else if (val < 0)
					{
						ntrun++;
						val = 0;
					}
					px |= val << i*8;
				}
				ret.setRGB(w, h, px);
			}
		if (ntrun != 0)
			System.err.println("Blur: Warning, " + ntrun + " pixels truncated. Output may be incorrect");
		return ret;
	}
	public static void main(String []args)
	{
		Scanner sc = new Scanner(System.in);
		String s;
		BufferedImage in, out;
		long t;

		System.out.println("Enter image path");
		s = sc.nextLine();

		try {
			in = ImageIO.read(new File(s));

		} catch (IOException e) {
			System.err.println("Read error");
			return;
		}
		t = System.currentTimeMillis();
		System.out.println("Blurring...");
		out = blur(in, getKernel1D(1));
		t = System.currentTimeMillis()-t;
		System.out.println("Blur took " + t + "ms");
		System.out.print("Writing file ...");
		try {
			ImageIO.write(out, "png", new File("out.png"));
		} catch (IOException e) {
			System.err.println("Write error");
			return;
		}
		System.out.println(" out.png written.");

		/*
		double[] k = getKernel1D(2);
		System.out.println("Benchmarking...");
		System.gc();
		t = System.currentTimeMillis();
		for (int i = 0; i < 200; i++)
			blur(in, k);
		System.gc();
		t = System.currentTimeMillis() - t;
		System.out.println("Benchmark: " + t/200 + "ms per operation");
		//*/
	}
}
