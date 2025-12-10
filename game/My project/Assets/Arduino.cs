using System;
using System.IO.Ports;
using System.Threading;
using UnityEngine;

public class Arduino : MonoBehaviour
{
    public string connectionPort;

    private SerialPort _arduinoStream;
    private Thread _arduinoThread;
    private string _receivedMessage;
    private bool _receiveNewMessage;

    void Start()
    {
        if (connectionPort != null)
        {
            _arduinoStream = new SerialPort(connectionPort, 9600);
            _arduinoStream.ReadTimeout = 10000;

            try
            {
                _arduinoStream.Open();
                _arduinoThread = new Thread(ReadArduinoStream);
                _arduinoThread.Start();
                Debug.Log("Arduino thread started");
            }
            catch
            {
                Debug.Log("Arduino thread could not be started");
            }
        }
    }

    void ReadArduinoStream()
    {
        while (_arduinoStream.IsOpen)
        {
            try
            {
                _receivedMessage = _arduinoStream.ReadLine();
                _receiveNewMessage = true;
            }
            catch (Exception ex)
            {
                Debug.Log(ex.Message);
            }
        }
    }

    void Update()
    {
        if (_receiveNewMessage)
        {
            Debug.Log(_receivedMessage);
            _receiveNewMessage = false;
        }
    }

    void OnApplicationQuit()
    {
        if (_arduinoStream != null && _arduinoStream.IsOpen)
        {
            _arduinoStream.Close();
        }
    }
}