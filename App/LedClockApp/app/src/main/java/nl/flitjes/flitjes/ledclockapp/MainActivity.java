package nl.flitjes.flitjes.ledclockapp;

import android.graphics.Paint;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class MainActivity extends AppCompatActivity implements ColorPickerDialog.OnColorChangedListener{
    private Paint mPaint = new Paint();
    private LedClock.COLOR c_id;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }
    public void showTimePickerDialog(View v) {
        DialogFragment newFragment = new TimePickerFragment();
        newFragment.show(getSupportFragmentManager(), "timePicker");
    }

    public void showHourColorPicker(View v) {
        c_id = LedClock.COLOR.HOUR;
        new ColorPickerDialog(this, this, mPaint.getColor()).show();

    }
    public void showMinColorPicker(View v) {
        c_id = LedClock.COLOR.MINUTE;
        new ColorPickerDialog(this, this, mPaint.getColor()).show();
    }

    @Override
    public void colorChanged(int color) {
        LedClock l = new LedClock();
        l.setColor(c_id, color);
    }
}

