The native device now exposes the recovered QFile error separately from sticky QTextStream failure. A successful close can clear the file error after retrying buffered bytes; it can also clear a later aboutToClose failure when the earlier, saved byte-buffer flush succeeded. The original project writer checks this file error after Close.

The exact branches, original addresses, evidence hashes, frozen source hashes and six focused test names are retained in [the machine-readable receipt](wave3-file-device-error-contract.json). The coordinator owns compilation and tests. No original process, DLL execution or UI was used.

The new locked-file cases distinguish recovery before Close from a lock retained during Close, including the saved-initial-flush branch. Existing successful close-tail, Stream-destructor-tail, buffering boundary, failed-open and direct-write coverage remains. Native truncate failure is still ignored as in the original.
