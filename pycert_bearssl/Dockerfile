
FROM python:3.10.1

RUN pip3 install pyOpenSSL click certifi

WORKDIR /pycert_bearssl

COPY src/ .

CMD ["python3", "pycert_bearssl.py", "convert", "--no-search", "-",  "-o", "-"]

