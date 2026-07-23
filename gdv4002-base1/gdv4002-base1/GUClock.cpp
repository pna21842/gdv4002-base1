
#include "GUClock.h"
#include <Windows.h>
#include <iostream>

using namespace std;

namespace CoreStructures {

	//
	// Private class to track frames-per-second (which varies non-linearly) and seconds-per-frame
	//

	class GUFrameCounter : public GUObject {

	private:

		int					frame;

		gu_seconds			fpsRefTimeIndex;

		unsigned long		fpsCounts;

		int					_framesPerSecond, _minimumFPS, _maximumFPS, _averageFPS;
		gu_seconds			_secondsPerFrame, _minimumSPF, _maximumSPF, _averageSPF;

	public:
		// test alternate FPS and SPF model
		int					_alt_frame;
		gu_seconds			_alt_fpsRefTimeIndex;
		unsigned long		_alt_fpsCounts;
		double				_alt_framesPerSecond, _alt_minimumFPS, _alt_maximumFPS, _alt_averageFPS;
		gu_seconds			_alt_secondsPerFrame, _alt_minimumSPF, _alt_maximumSPF, _alt_averageSPF;


	public:

		GUFrameCounter(gu_time_index baseTime = 0) {

			resetCounter(baseTime);
		}

		void resetCounter(gu_time_index resetTime = 0) {

			frame = 0;

			fpsRefTimeIndex = 0.0;

			fpsCounts = 0;

			_framesPerSecond = _minimumFPS = _maximumFPS = _averageFPS = 0;
			_secondsPerFrame = _minimumSPF = _maximumSPF = _averageSPF = 0.0;


			//
			// test alternate frame counter model
			//

			_alt_frame = 0;

			_alt_fpsRefTimeIndex = 0.0;
			
			_alt_fpsCounts = 0;
			
			_alt_framesPerSecond = _alt_minimumFPS = _alt_maximumFPS = _alt_averageFPS = 0.0;
			_alt_secondsPerFrame = _alt_minimumSPF = _alt_maximumSPF = _alt_averageSPF = 0.0;
		}


		void updateFrameCounterForElaspsedTime(gu_seconds gameTimeElapsed) {

			// Update frame counter
			frame++;
			_alt_frame++;


			//
			// Process current (main) time_delta model
			//

			gu_seconds time_delta = gameTimeElapsed - fpsRefTimeIndex;

			// Check if 1 second of game time has elapsed - if so modify fps
			if (time_delta >= 1.0) {

				_framesPerSecond = frame; // Get current frames per second (frame/1.0)
				_secondsPerFrame = 1000.0 / (gu_seconds)_framesPerSecond; // Get current (milli)seconds per frame

				if (fpsCounts==0) {

					// First iteration so initialise maximum, minimum and average fps and seconds per frame
					_minimumFPS = _maximumFPS = _averageFPS = _framesPerSecond;
					_minimumSPF = _maximumSPF = _averageSPF = _secondsPerFrame;
		
				} else {

					// Update maximum, minimum and average fps
					if (_framesPerSecond < _minimumFPS)
						_minimumFPS = _framesPerSecond;
					else if (_framesPerSecond > _maximumFPS)
						_maximumFPS = _framesPerSecond;

					_averageFPS += _framesPerSecond;

					// Update maximum, minimum and averse (milli)seconds per frame
					if (_secondsPerFrame < _minimumSPF)
						_minimumSPF = _secondsPerFrame;
					else if (_secondsPerFrame > _maximumSPF)
						_maximumSPF = _secondsPerFrame;

					_averageSPF += _secondsPerFrame;
				}

				// Reset frame counter for next iteration
				frame = 0;
				fpsRefTimeIndex += 1.0;

				fpsCounts++;
			}
		
		
			//
			// Process alt time_delta model
			//

			gu_seconds _alt_time_delta = gameTimeElapsed - _alt_fpsRefTimeIndex;

			if (_alt_time_delta >= 1.0) {

				// _framesPerSecond assumes time delta = 1.0 second but what if this is not the case (although it is likely gameTimeElapsed - fpsRefTimeIndex is approximately 1.0 seconds this does not take into account fractions frames-per-second for time complex operations being timed by GUClock.
				_alt_framesPerSecond = (double)_alt_frame / _alt_time_delta;
				_alt_secondsPerFrame = _alt_time_delta / (double)_alt_frame;

				if (_alt_fpsCounts==0) {

					// First iteration so initialise maximum, minimum and average fps and seconds per frame
					_alt_minimumFPS = _alt_maximumFPS = _alt_averageFPS = _alt_framesPerSecond;
					_alt_minimumSPF = _alt_maximumSPF = _alt_averageSPF = _alt_secondsPerFrame;
		
				} else {

					// Update maximum, minimum and average fps
					if (_alt_framesPerSecond < _alt_minimumFPS)
						_alt_minimumFPS = _alt_framesPerSecond;
					else if (_alt_framesPerSecond > _alt_maximumFPS)
						_alt_maximumFPS = _alt_framesPerSecond;

					_alt_averageFPS += _alt_framesPerSecond;

					// Update maximum, minimum and averse (milli)seconds per frame
					if (_alt_secondsPerFrame < _alt_minimumSPF)
						_alt_minimumSPF = _alt_secondsPerFrame;
					else if (_alt_secondsPerFrame > _alt_maximumSPF)
						_alt_maximumSPF = _alt_secondsPerFrame;

					_alt_averageSPF += _alt_secondsPerFrame;
				}

				// Reset frame counter for next iteration
				_alt_frame = 0;
				
				// note:  if a process takes significantly longer than 1 second, incrementing ref time index by 1.0 second means it lags behind the actual game time index so the next update call will track a low number of FPS (perhaps just 1).  So _alt_fpsRefTimeIndex resets to the actual game time so we start tracking frames from this point.  This means the clock will be more precise for time complex operations (not likely in a game environment though)
				_alt_fpsRefTimeIndex = gameTimeElapsed;

				_alt_fpsCounts++;
			}

		}
		

		int framesPerSecond() const { return _framesPerSecond; }
		int minimumFPS() const { return _minimumFPS; }
		int maximumFPS() const { return _maximumFPS; }
		gu_seconds averageFPS() const { return ((gu_seconds)_averageFPS)/(gu_seconds)fpsCounts; }
		gu_seconds secondsPerFrame() const { return _secondsPerFrame; }
		gu_seconds minimumSPF() const { return _minimumSPF; }
		gu_seconds maximumSPF() const { return _maximumSPF; }
		gu_seconds averageSPF() const { return _averageSPF/(gu_seconds)fpsCounts; }

		double altFramesPerSecond() const { return _alt_framesPerSecond; }
		double altMinimumFPS() const { return _alt_minimumFPS; }
		double altMaximumFPS() const { return _alt_maximumFPS; }
		gu_seconds altAverageFPS() const { return ((gu_seconds)_alt_averageFPS)/(gu_seconds)_alt_fpsCounts; }
		gu_seconds altSecondsPerFrame() const { return _alt_secondsPerFrame; }
		gu_seconds altMinimumSPF() const { return _alt_minimumSPF; }
		gu_seconds altMaximumSPF() const { return _alt_maximumSPF; }
		gu_seconds altAverageSPF() const { return _alt_averageSPF/(gu_seconds)_alt_fpsCounts; }

	};



	//
	// GUClock implementation
	//


	// Class method implementation

	gu_time_index GUClock::actualTime() {

		gu_time_index t;

		QueryPerformanceCounter((LARGE_INTEGER*)&t);

		return t;
	}



	// Private method implementation
	
	void GUClock::resetClockAttributes() {

		baseTime = GUClock::actualTime();

		prevTimeIndex = baseTime;

		deltaTime = 0;

		stopTimeIndex = 0;
		totalStopTime = 0;

		_clockStopped = false;
	}


	// Force clock into stopped state (only called in constructor).  The clock cannot be started (see start() method above)
	void GUClock::invalidateClock() {

		baseTime = 0;
		prevTimeIndex = 0;
		currentTimeIndex = 0;

		deltaTime = 0;

		stopTimeIndex = 0;
		totalStopTime = 0;

		_clockStopped = true;

		frameCounter = NULL;
	}


	// Convert time interval to gu_seconds
	gu_seconds GUClock::convertTimeIntervalToSeconds(gu_time_interval t) const {

		return (gu_seconds)t * timeRecip;
	}



	// Public method implementation
	
	GUClock::GUClock(void) {

		QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFrequency);
	
		if (performanceFrequency!=0) {

			timeRecip = 1.0 / (gu_seconds)performanceFrequency; // valid high-performance counter present

			resetClockAttributes();
			frameCounter = new GUFrameCounter();
	
		} else {
		
			cout << "clock initialisation error" << endl;

			timeRecip = 0.0; // high-performance counter not present - timeRecip = 0 means clock cannot be started

			invalidateClock();
		}
	}


	GUClock::~GUClock(void) {

		if (frameCounter)
			frameCounter->release();
	}


	// clock update methods

	void GUClock::start() {

		// Only allow clock to start if a valid performance frequency is present
		if (_clockStopped && performanceFrequency!=0) {

			gu_time_index		restartTimeIndex = GUClock::actualTime();

			totalStopTime += restartTimeIndex - stopTimeIndex;
			prevTimeIndex = restartTimeIndex;

			_clockStopped = false;
		}
	}


	void GUClock::stop() {

		if (!_clockStopped)
		{
			stopTimeIndex = GUClock::actualTime();

			_clockStopped = true;
		}
	}


	void GUClock::tick() {

		if (_clockStopped) {

			deltaTime = 0;
			return;
		}

		currentTimeIndex = GUClock::actualTime();
		deltaTime = currentTimeIndex - prevTimeIndex;
		prevTimeIndex = currentTimeIndex;

		if (frameCounter)
			frameCounter->updateFrameCounterForElaspsedTime(convertTimeIntervalToSeconds((currentTimeIndex - baseTime) - totalStopTime));
	}


	void GUClock::reset() {

		resetClockAttributes();

		if (frameCounter)
			frameCounter->resetCounter();
	}


	// clock query methods

	gu_seconds GUClock::actualTimeElapsed() const {

		return convertTimeIntervalToSeconds(GUClock::actualTime() - baseTime);
	}


	gu_seconds GUClock::gameTimeElapsed() const {

		return convertTimeIntervalToSeconds((((_clockStopped) ? stopTimeIndex : GUClock::actualTime()) - baseTime) - totalStopTime);
	}


	gu_seconds GUClock::gameTimeDelta() const {

		return convertTimeIntervalToSeconds(deltaTime);
	}


	bool GUClock::clockStopped() const {

		return _clockStopped;
	}


	void GUClock::reportTimingData() const {

		/*cout << "max FPS = " << maximumFPS() << endl;
		cout << "min FPS = " << minimumFPS() << endl;
		cout << "average FPS = " << averageFPS() << endl;
	
		cout << "max SPF = " << (maximumSPF() / 1000.0) << endl;
		cout << "min SPF = " << (minimumSPF() / 1000.0) << endl;
		cout << "average SPF = " << (averageSPF() / 1000.0) << endl;*/

		if (frameCounter) {

			cout << "max FPS = " << frameCounter->altMaximumFPS() << endl;
			cout << "min FPS = " << frameCounter->altMinimumFPS() << endl;
			cout << "average FPS = " << frameCounter->altAverageFPS() << endl;
	
			cout << "max SPF = " << (frameCounter->altMaximumSPF() /*/ 1000.0*/) << endl;
			cout << "min SPF = " << (frameCounter->altMinimumSPF() /*/ 1000.0*/) << endl;
			cout << "average SPF = " << (frameCounter->altAverageSPF() /*/ 1000.0*/) << endl;
		}
	}


	int GUClock::framesPerSecond() const {
		
		return (frameCounter) ? frameCounter->altFramesPerSecond() : 0;
	}


	int GUClock::minimumFPS() const {

		return (frameCounter) ? frameCounter->altMinimumFPS() : 0;
	}


	int GUClock::maximumFPS() const {

		return (frameCounter) ? frameCounter->altMaximumFPS() : 0;
	}


	gu_seconds GUClock::averageFPS() const {

		return (frameCounter) ? frameCounter->altAverageFPS() : 0.0;
	}


	gu_seconds GUClock::secondsPerFrame() const {

		return (frameCounter) ? frameCounter->altSecondsPerFrame() : 0.0;
	}


	gu_seconds GUClock::minimumSPF() const {

		return (frameCounter) ? frameCounter->altMinimumSPF() : 0.0;
	}


	gu_seconds GUClock::maximumSPF() const {

		return (frameCounter) ? frameCounter->altMaximumSPF() : 0.0;
	}


	gu_seconds GUClock::averageSPF() const {

		return (frameCounter) ? frameCounter->altAverageSPF() : 0.0;
	}

}
