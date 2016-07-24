package nl.flitjes.flitjes.ledclockapp;

import android.os.AsyncTask;
import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;

/**
 * Created by flitjesdev on 2/21/16.
 */
public class LedClock {
    public enum COLOR {
        MINUTE,
        HOUR;};
    byte[] send_data = new byte[1024];
    int send_data_length = 0;
    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public void setColor (COLOR c, int color){
        String DATA_TRANSFER;
        ByteBuffer b = ByteBuffer.allocate(4);
        b.putInt(color);
        byte[] result = b.array();

        DATA_TRANSFER = String.format("setcolor=%d%02x%02x%02x\n", c.ordinal(), result[1], result[2], result[3]);
        sendUDPString(DATA_TRANSFER);

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Past sleep");
        DATA_TRANSFER = String.format("start\n");
        sendUDPString(DATA_TRANSFER);

    }

    public void sendUDPString(String DATA_TRANSFER){
        int PORT = 12345;
        String IP="192.168.4.1";

        int offset = DATA_TRANSFER.length();
        for (int i = 0; i < offset; i++){
            send_data[i] = DATA_TRANSFER.getBytes()[i];
        }

        String str = new String(send_data);
        Log.d("Test", str);
        send_data_length = offset;
        AsyncTask task = new AsyncClient(IP, PORT).execute(send_data);
    }

    public void setTime(int hour, int minute){
        String DATA_TRANSFER;

        DATA_TRANSFER = String.format("time=%d:%d:00\n", hour, minute);
        sendUDPString(DATA_TRANSFER);

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Past sleep");
        DATA_TRANSFER = String.format("start\n");
        sendUDPString(DATA_TRANSFER);

    }



    class AsyncClient extends AsyncTask<byte[], Void, byte[]> {
        int PORT;
        String IP;

    private  final String TAG = AsyncClient.class.getName();
    public AsyncClient(String ip, int port) {
        super();
        PORT=port;
        IP=ip;
    }

    protected void onPreExecute(){
        Log.d(TAG, "On preExceute...");
    }

    protected byte[] doInBackground(byte[]...arg0) {
        byte[] receiveData = new byte[1024];
        Log.d(TAG, "On doInBackground...");

        DatagramSocket client_socket = null;
        try {
            client_socket = new DatagramSocket(PORT);
            InetAddress IPAddress =  InetAddress.getByName(IP);

            DatagramPacket send_packet = new DatagramPacket(send_data, send_data_length, IPAddress, PORT);
            client_socket.send(send_packet);

                /*DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                client_socket.receive(receivePacket);
                receiveData = receivePacket.getData();*/

            client_socket.close();
        } catch (SocketException e) {
            e.printStackTrace();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
            /*String str = new String(receiveData);

            Log.d(TAG,str);*/
        return receiveData;
    }

    protected void onProgressUpdate(Integer...a){
        Log.d(TAG,"You are in progress update ... " + a[0]);
    }

    protected void onPostExecute(String result) {
        Log.d(TAG,result);
    }
}

}
